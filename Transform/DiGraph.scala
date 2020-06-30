package DiGraph

import java.io.{File, FileWriter}

import firrtl.ir._
import firrtl.{Addw, ChirrtlToHighFirrtl, CircuitState, Dshlw, HighFirrtlToMiddleFirrtl, IRToWorkingIR, LowFirrtlOptimization, LowForm, MiddleFirrtlToLowFirrtl, ResolveAndCheck, Subw, Transform, Utils, WRef, WSubAccess, WSubField, bitWidth}

import scala.io.Source
import firrtl.Mappers._
import firrtl.PrimOps.{Add, And, Andr, AsClock, AsSInt, AsUInt, Bits, Cat, Cvt, Div, Dshl, Dshr, Eq, Geq, Gt, Head, Leq, Lt, Mul, Neg, Neq, Not, Or, Orr, Pad, Rem, Shl, Shr, Sub, Tail, Xor, Xorr}
import firrtl.stage.{Forms, TransformManager}

import scala.collection.mutable

/*
* name: the name of this signal
* id: the index of MP for String
* cppType: UInt, SInt, Float, ...
* cppWidth: len
* dir: If is a Port
* typ: Reg wire Mux Validif Node
* ext: dynamic  i.e. Uint : 10
* */
class Node(var name:String,
           var id:String,
           var cppType:String,
           var cppWidth:BigInt,
           var dir:String,
           var tpe:String,
           var expr:String,
          )

class Edge(var from:String,
           var to:String,
          )

class Graph {
    // name -> Node
    var NodeMP = mutable.Map[String, Node]()
    // edge collection
    var EdgeSet = mutable.Seq[Edge]()
    // the name->id
    var MP = mutable.Map[String,Int]()
    // the number of Node
    var cnt = 0

    def Output()={
        NodeMP.foreach((e)=>{
            val t = e._2
            printf("%s %s %s %s %s %s\n",t.id,t.tpe,t.cppType,t.dir,t.name,t.expr)
        })
    }

    def addNode(name:String, cppType:String,cppWidth:BigInt, dir:String,tpe:String, expr:String):Node={
        MP += (name -> cnt)
        val node = new Node(name, s"${cnt}", cppType, cppWidth, dir, tpe, expr)
        NodeMP += (name -> node)
        cnt += 1
        node
    }

    def serialize: String = {
        return  "\n"
    }

}


class AnalyzeCircuit extends Transform {
//    def inputForm = LowForm
//
//    def outputForm = LowForm

    val graph = new Graph()
    var flag = false

    def chunkLitString(litStr: String, chunkWidth: Int = 16): Seq[String] = {
        if (litStr.length < chunkWidth) Seq(litStr)
        else chunkLitString(litStr.dropRight(chunkWidth)) ++ Seq(litStr.takeRight(chunkWidth))
    }

    // NOTE: assuming no large UIntLiteral is negative
    def splatLargeLiteralIntoRawArray(value: BigInt, width: BigInt): String = {
        val rawHexStr = value.toString(16)
        val isNeg = value < 0
        val asHexStr = if (isNeg) rawHexStr.tail else rawHexStr
        val arrStr = chunkLitString(asHexStr) map {
            "0x" + _
        } mkString (",")
        val leadingNegStr = if (isNeg) "(uint64_t) -" else ""
        val numWords = (width + 63) / 64
        s"std::array<uint64_t,$numWords>({$leadingNegStr$arrStr})"
    }

    def execute(state: CircuitState): CircuitState = {
        val circuit = state.circuit
        circuit map walkModule
        graph.Output()
        state
    }

    def walkModule(m: DefModule): DefModule = {

        m match {
            case Module(info, name, ports, body) =>
                ports.foreach(e => {
                    if (e.name != "clock" && e.name != "reset") {
                        val tuple = Main.genCppType(e.tpe)
                        graph.addNode(e.name,tuple._1,tuple._2,e.direction.toString,"Port","Node")
                    }
                })
            case ExtModule(info, name, ports, defname, params) =>
                m
        }
        m map walkStatement(graph)
    }

    def walkStatement(graph: Graph)(s: Statement): Statement = {

        s match {
            case r: DefRegister =>
                val tuple = Main.genCppType(r.tpe)
                graph.addNode(r.name,tuple._1,tuple._2,"None","Reg","None")

            case DefNode(info, name, value) =>
                val t = parseExpr(value)
                val t2 = graph.addNode(name,"12",32,"None","Node","None")

                t2.cppType = t.cppType
                t2.cppWidth = t.cppWidth
                graph.EdgeSet :+= new Edge(name, t.name)

            case DefWire(info, name, tpe) =>
                val tuple = Main.genCppType(tpe)
                graph.addNode(name,tuple._1,tuple._2,"None","wire","None")

            case Stop(info, ret, clk, en) =>
            case Conditionally(info, pred, conseq, alt) =>
            case m: DefMemory =>
            case instance: DefInstance =>
            case connect: Connect => //等式左边
                var node: Node = new Node("1", "1", "1", 1, "1", "1", "1")
                connect.loc match {
                    case WRef(name, tpe, kind, flow) => node = graph.NodeMP(name)
                    case SubAccess(expr, index, tpe) => node = parseExpr(connect.loc)
                    case _ => throw new Exception("Don't Support this syntax.")
                }
                val node2 = parseExpr(connect.expr)
                graph.EdgeSet:+=new Edge(node.name,node2.name)

            case partialConnect: PartialConnect =>
            case invalid: IsInvalid =>
            case attach: Attach =>
            case stop: DefInstance =>
            case p: Print =>
                val formatters = "(%h)|(%x)|(%d)|(%ld)".r.findAllIn(p.string.serialize).toList
                val argWidths = p.args map { e: Expression => bitWidth(e.tpe) }
                if (!(argWidths forall {
                    _ <= 64
                })) throw new Exception(s"Can't print wide signals")
                val replacements = formatters zip argWidths map { case (format, width) =>
                    if (format == "%h" || format == "%x") {
                        val printWidth = math.ceil((width / 4).toDouble).toInt
                        (format, s"""%0${printWidth}" PRIx64 """")
                    } else {
                        val printWidth = math.ceil(math.log10((1l << width.toInt).toDouble)).toInt
                        (format, s"""%${printWidth}" PRIu64 """")
                    }
                }
                val formatString = replacements.foldLeft(p.string.serialize) {
                    case (str, (searchFor, replaceWith)) => str.replaceFirst(searchFor, replaceWith)
                }
                val printfArgs = Seq(s""""$formatString"""") ++
                  (p.args map { arg => s"${emitExprWrap(arg)}.as_single_word()" })
                Seq(s"if (UNLIKELY(done_reset && update_registers && verbose && ${emitExprWrap(p.en)})) printf(${printfArgs mkString (", ")});")
            case _ =>
        }
        s map walkStatement(graph)
    }


    def parseExpr(e: Expression): Node = {
        e match {
            case WRef(name, tpe, _, _) =>
                graph.NodeMP(name)
            case u: UIntLiteral => {
                val maxIn64Bits = (BigInt(1) << 64) - 1
                val width = bitWidth(u.tpe)
                val asHexStr = u.value.toString(16)
                var res = "1"
                if ((width <= 64) || (u.value <= maxIn64Bits)) res = s"UInt<$width>(0x$asHexStr)"
                else res = s"UInt<$width>(${splatLargeLiteralIntoRawArray(u.value, width)})"
                graph.addNode(s"Uint${graph.cnt}",s"UInt<$width>",width,"None","UIntLiteral",res)
            }
            case u: SIntLiteral => {
                val width = bitWidth(u.tpe)
                var res = "1"
                if (width <= 64) res = s"SInt<$width>(${u.value.toString(10)})"
                else res = s"SInt<$width>(${splatLargeLiteralIntoRawArray(u.value, width)})"
                graph.addNode(s"Sint${graph.cnt}",s"SInt<$width>",width,"None","SIntLiteral",res)
            }
            case m: Mux => {
                //println(m)
                val condNode = parseExpr(m.cond)
                val tvalNode = parseExpr(m.tval)
                val fvalNode = parseExpr(m.fval)
                val t = graph.addNode(s"Mux${graph.cnt}","Mux",0,"None","Mux",s"${condNode.name} ? ${tvalNode.name} : ${fvalNode.name}")
                graph.EdgeSet:+=new Edge(t.name , condNode.name)
                graph.EdgeSet:+=new Edge(t.name , tvalNode.name)
                graph.EdgeSet:+=new Edge(t.name , fvalNode.name)
                t
            }
            case vif: ValidIf => {
                //println(m)
                val condNode = parseExpr(vif.cond)
                val tvalNode = parseExpr(vif.value)
                val t = graph.addNode(s"ValidIf${graph.cnt}","ValidIf",0,"None","ValidIf",s"${condNode.name} ? ${tvalNode.name}")
                graph.EdgeSet:+=new Edge(t.name , condNode.name)
                graph.EdgeSet:+=new Edge(t.name , tvalNode.name)
                t
            }
            case w: WSubField =>  graph.NodeMP(s"${emitExpr(w.expr)}_${w.name}")
            case w: WSubAccess =>
                val tuple = Main.genCppType(w.tpe)
                val t = graph.addNode(s"WSubAccess${graph.cnt}",tuple._1,tuple._2,"None","WSubAccess",s"${emitExpr(w.index)}")
                graph.EdgeSet:+=new Edge(t.name,emitExpr(w.expr))
//                s"${emitExpr(w.expr)}[${emitExprWrap(w.index)}.as_single_word()]"
                t

            case p: DoPrim =>
                p.op match {
                    case Add =>
                        val t = graph.addNode(s"Add${graph.cnt}","None",0,"None","Add","None");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Addw =>
                        val t = graph.addNode(s"Add${graph.cnt}","None",0,"None","Add","None");
                        val l = parseExpr(p.args(0))
                        val r = parseExpr(p.args(1))
                        graph.EdgeSet:+= new Edge(t.name,l.name)
                        graph.EdgeSet:+= new Edge(t.name,r.name)
                        graph.EdgeSet:+= new Edge(l.name,t.name)
                        t
                    case Sub =>
                        val t = graph.addNode(s"Sub${graph.cnt}","None",0,"None","Sub","None");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Subw => val t = graph.addNode(s"Add${graph.cnt}","None",0,"None","Add","None");
                        val l = parseExpr(p.args(0))
                        val r = parseExpr(p.args(1))
                        graph.EdgeSet:+= new Edge(t.name,l.name)
                        graph.EdgeSet:+= new Edge(t.name,r.name)
                        graph.EdgeSet:+= new Edge(l.name,t.name)
                        t

                    case Mul => val t = graph.addNode(s"Mul${graph.cnt}","None",0,"None","Mul","*");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Div => val t = graph.addNode(s"Div${graph.cnt}","None",0,"None","Div","/");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Rem => val t = graph.addNode(s"Rem${graph.cnt}","None",Integer.MAX_VALUE,"None","Rem","%");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth.min(t.cppWidth)
                            t.cppType = e.cppType
                        })
                        t
                    case Lt =>
                        val t = graph.addNode(s"Lt${graph.cnt}","bool",1,"None","Lt","<");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                        })
                        t
                    case Leq => val t = graph.addNode(s"Leq${graph.cnt}","bool",1,"None","Leq","<=");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                        })
                        t
                    case Gt =>   val t = graph.addNode(s"Gt${graph.cnt}","bool",1,"None","Gt",">");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                        })
                        t
                    case Geq => val t = graph.addNode(s"Geq${graph.cnt}","bool",1,"None","Geq",">=");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                        })
                        t
                    case Eq =>  val t = graph.addNode(s"Eq${graph.cnt}","bool",1,"None","Eq","==");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                        })
                        t
                    case Neq => val t = graph.addNode(s"Neq${graph.cnt}","bool",1,"None","Neq","!=");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                        })
                        t
                    case Pad =>
                        val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"Pad${graph.cnt}",node.cppType,bitWidth(p.tpe),"None","Pad","Pad");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                        //s"${emitExprWrap(p.args.head)}.pad<${}>()"
                    case AsUInt => val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"AsUInt${graph.cnt}",s"UInt<${bitWidth(p.tpe)}>",bitWidth(p.tpe),"None","AsUInt","AsUInt");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case AsSInt => val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"AsSInt${graph.cnt}",s"SInt<${bitWidth(p.tpe)}>",bitWidth(p.tpe),"None","AsSInt","AsSInt");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case AsClock => throw new Exception("AsClock unimplemented!")
                    case Shl =>
                        val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"Shl${graph.cnt}",node.cppType,bitWidth(p.tpe)+p.consts.head.toInt,"None","Shl",s"${p.consts.head.toInt}");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    //case Shlw => s"${emitExprWrap(p.args.head)}.shlw<${p.consts.head.toInt}>()"
                    case Shr =>
                        val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"Shr${graph.cnt}",node.cppType,bitWidth(p.tpe)-p.consts.head.toInt,"None","Shr",s"${p.consts.head.toInt}");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case Dshl => val t = graph.addNode(s"Div${graph.cnt}","None",0,"None","Div","/");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Dshr => val t = graph.addNode(s"Div${graph.cnt}","None",0,"None","Div","/");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Dshlw => s"${emitExprWrap(p.args(0))}.dshlw(${emitExpr(p.args(1))})"
                    case Cvt => s"${emitExprWrap(p.args.head)}.cvt()"
                    case Neg => val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"AsSInt${graph.cnt}",s"SInt<${bitWidth(p.tpe)}>",bitWidth(p.tpe),"None","AsSInt","AsSInt");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case Not => val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"AsSInt${graph.cnt}",s"SInt<${bitWidth(p.tpe)}>",bitWidth(p.tpe),"None","AsSInt","AsSInt");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case And => val t = graph.addNode(s"Div${graph.cnt}","None",0,"None","Div","/");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Or =>  val t = graph.addNode(s"Div${graph.cnt}","None",0,"None","Div","/");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Xor => val t = graph.addNode(s"Div${graph.cnt}","None",0,"None","Div","/");
                        p.args map parseExpr foreach(e => {
                            graph.EdgeSet:+= new Edge(t.name,e.name)
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Andr => val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"AsSInt${graph.cnt}",s"SInt<${bitWidth(p.tpe)}>",bitWidth(p.tpe),"None","AsSInt","AsSInt");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case Orr =>  val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"AsSInt${graph.cnt}",s"SInt<${bitWidth(p.tpe)}>",bitWidth(p.tpe),"None","AsSInt","AsSInt");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case Xorr => val node = graph.NodeMP(emitExprWrap(p.args.head))
                        val t = graph.addNode(s"AsSInt${graph.cnt}",s"SInt<${bitWidth(p.tpe)}>",bitWidth(p.tpe),"None","AsSInt","AsSInt");
                        graph.EdgeSet:+= new Edge(t.name,node.name)
                        t
                    case Cat => s"${emitExprWrap(p.args(0))}.cat(${emitExpr(p.args(1))})"
                    case Bits => s"${emitExprWrap(p.args.head)}.bits<${p.consts(0).toInt},${p.consts(1).toInt}>()"
                    case Head => s"${emitExprWrap(p.args.head)}.head<${p.consts.head.toInt}>()"
                    case Tail => s"${emitExprWrap(p.args.head)}.tail<${p.consts.head.toInt}>()"
                }
            case _ => throw new Exception(s"Don't yet support $e")

        }
    }

    def emitExprWrap(e: Expression): String = e match {
        case DoPrim(_, _, _, _) | Mux(_, _, _, _) => s"(${emitExpr(e)})"
        case _ => emitExpr(e)
    }

    def emitExpr(e: Expression): String = {
        e match {
            case WRef(name, tpe, _, _) =>
                name
            case u: UIntLiteral => {
                val maxIn64Bits = (BigInt(1) << 64) - 1B

                val width = bitWidth(u.tpe)
                val asHexStr = u.value.toString(16)
                if ((width <= 64) || (u.value <= maxIn64Bits)) s"UInt<$width>(0x$asHexStr)"
                else s"UInt<$width>(${splatLargeLiteralIntoRawArray(u.value, width)})"
            }
            case u: SIntLiteral => {
                val width = bitWidth(u.tpe)
                if (width <= 64) s"SInt<$width>(${u.value.toString(10)})"
                else s"SInt<$width>(${splatLargeLiteralIntoRawArray(u.value, width)})"
            }
            case w: WSubField => s"${emitExpr(w.expr)}_${w.name}"
            case w: WSubAccess => s"${emitExpr(w.expr)}[${emitExprWrap(w.index)}]"
            case _ => throw new Exception(s"Don't yet support $e")

        }
    }
}
object Main{
    def genCppType(tpe: Type):(String,BigInt) = tpe match {
        case UIntType(IntWidth(w)) => (s"UInt<$w>",w)
        case SIntType(IntWidth(w)) => (s"SInt<$w>",w)
        case _ => throw new Exception(s"No CPP type implemented for $tpe")
    }

    def main(array: Array[String]) = {
        generate(new File("./firrtl/gcd.fir"))
    }

    def generate(inputFile: File) {
        val circuit = firrtl.Parser.parse(Source.fromFile(inputFile).getLines,
            firrtl.Parser.IgnoreInfo)
        val  topName = circuit.main
        var state = CircuitState(circuit,firrtl.HighForm)

        val analyzeCircuit = new AnalyzeCircuit()

        val ChirrtlToWorkingIR = new TransformManager(Forms.MinimalHighForm, Forms.ChirrtlForm)
        val irToWorkingIR = new TransformManager(Forms.WorkingIR, Forms.MinimalHighForm)
        state = ChirrtlToWorkingIR.execute(state)
        state = irToWorkingIR.execute(state)
        val compiler = new firrtl.stage.transforms.Compiler(
            targets=firrtl.stage.Forms.LowForm,
            currentState=firrtl.stage.Forms.MinimalHighForm
        )
        state = compiler.transform(state)
        analyzeCircuit.execute(state)
    }
}