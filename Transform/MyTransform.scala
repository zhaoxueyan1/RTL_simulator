// See LICENSE for license details.


import java.io.{File, FileWriter}


import firrtl.ir._
import firrtl.{Addw, ChirrtlToHighFirrtl, CircuitState, Dshlw, HighFirrtlToMiddleFirrtl, IRToWorkingIR, LowFirrtlOptimization, LowForm, MiddleFirrtlToLowFirrtl, ResolveAndCheck, Subw, Transform, Utils, WRef, WSubAccess, WSubField, bitWidth}

import scala.io.Source
import firrtl.Mappers._
import firrtl.PrimOps.{Add, And, Andr, AsClock, AsSInt, AsUInt, Bits, Cat, Cvt, Div, Dshl, Dshr, Eq, Geq, Gt, Head, Leq, Lt, Mul, Neg, Neq, Not, Or, Orr, Pad, Rem, Shl, Shr, Sub, Tail, Xor, Xorr}
import firrtl.stage.{Forms, TransformManager}

import scala.collection.mutable


class Graph {
    //  private var moduleName: Option[String] = None
//    private var RegSet = mutable.Seq[DefRegister]()
    var RegSet = mutable.Set[String]()
    var RSet = mutable.Seq[DefRegister]()
    private var NodeSet = mutable.Seq[DefNode]()
    private var InputPort = mutable.Seq[Port]()
    private var OutputPort = mutable.Seq[Port]()
    private var HeadMap = mutable.Map[String, String]()
    var DetailMap = mutable.Map[String, Expression]()
    var DetailStrMap = mutable.Map[String, String]()


    def PushReg(defRegister: DefRegister) = RegSet.add(defRegister.name)
//    def PushWire(defWire: DefWire)  =WireSet:+defWire
    def PushNode(defNode: DefNode)  =NodeSet:+defNode
    def addEdge(str1:String,str2:String)=HeadMap+=(str1->str2)
    def Output()={
        HeadMap.foreach(e=>{if(RegSet.contains(e._1)) println(e._1+" = "+e._2)})
    }
    def serialize: String = {
        return  "\n"
    }
}


class AnalyzeCircuit extends Transform {
    def inputForm = LowForm
    def outputForm = LowForm
    val graph = new Graph()

    def execute(state: CircuitState): CircuitState = {
        val circuit = state.circuit
        circuit map walkModule(graph)
        graph.Output()
        state
    }
    def walkModule(graph: Graph)(m: DefModule): DefModule = {
        //graph.Graph(m.name)
        m match {
            case Module(info, name, ports, body) =>
                ports.filter(e=>e.direction==Output).foreach(e=>graph.RegSet.add(e.name))
            case ExtModule(info, name, ports, defname, params)=>
                m
        }
        m map walkStatement(graph)
    }
    def chunkLitString(litStr: String, chunkWidth:Int = 16): Seq[String] = {
        if (litStr.size < chunkWidth) Seq(litStr)
        else chunkLitString(litStr.dropRight(chunkWidth)) ++ Seq(litStr.takeRight(chunkWidth))
    }

    // NOTE: assuming no large UIntLiteral is negative
    def splatLargeLiteralIntoRawArray(value: BigInt, width: BigInt): String = {
        val rawHexStr = value.toString(16)
        val isNeg = value < 0
        val asHexStr = if (isNeg) rawHexStr.tail else rawHexStr
        val arrStr = chunkLitString(asHexStr) map { "0x" + _} mkString(",")
        val leadingNegStr = if (isNeg) "(uint64_t) -" else ""
        val numWords = (width + 63) / 64
        s"std::array<uint64_t,$numWords>({$leadingNegStr$arrStr})"
    }

    def walkStatement(graph: Graph)(s: Statement): Statement = {

//        s map emitExpr

        s match {
            case DefRegister(info, name, tpe, clock, reset, init)=>
                graph.RegSet.add(name)
                //        println(name)
                //        println(tpe)
                s
            case DefNode(info, name, value)=>
//                println(name)
                //        println(value)
                graph.addEdge(name,emitExpr(value))
                graph.DetailMap+=(name->value)
                s
            case DefWire(info, name, value)=>
                //        println(name)
                //        println(value)
                s
            case Stop(info, ret, clk, en)=>
                s
            case Conditionally(info, pred, conseq, alt)=>
                //        println(s)
                //        println(conseq)
                s
            case m : DefMemory =>
                s
            case instance : DefInstance =>
                s
            case connect : Connect =>
//                println(connect)
                graph.addEdge(emitExpr(connect.loc),emitExpr(connect.expr))
                if (!graph.RegSet.contains(emitExpr(connect.loc))){
                    connect.loc match {
                        case WRef(name, tpe, kind, flow)=>
                            graph.DetailMap+=(name->connect.expr)
                    }
                }

                s
            case partialConnect : PartialConnect =>
                s
            case invalid : IsInvalid =>
                s
            case attach : Attach =>
                s
            case stop : DefInstance =>
                s
            case p : Print =>
                val formatters = "(%h)|(%x)|(%d)|(%ld)".r.findAllIn(p.string.serialize).toList
                val argWidths = p.args map {e: Expression => bitWidth(e.tpe)}
                if (!(argWidths forall { _ <= 64 })) throw new Exception(s"Can't print wide signals")
                val replacements = formatters zip argWidths map { case(format, width) =>
                    if (format == "%h" || format == "%x") {
                        val printWidth = math.ceil((width/4).toDouble).toInt
                        (format, s"""%0${printWidth}" PRIx64 """")
                    } else {
                        val printWidth = math.ceil(math.log10((1l<<width.toInt).toDouble)).toInt
                        (format, s"""%${printWidth}" PRIu64 """")
                    }
                }
                val formatString = replacements.foldLeft(p.string.serialize){
                    case (str, (searchFor, replaceWith)) => str.replaceFirst(searchFor, replaceWith)
                }
                val printfArgs = Seq(s""""$formatString"""") ++
                  (p.args map {arg => s"${emitExprWrap(arg)}.as_single_word()"})
                Seq(s"if (UNLIKELY(done_reset && update_registers && verbose && ${emitExprWrap(p.en)})) printf(${printfArgs mkString(", ")});")
                s
            case _ => s
        }
        s map walkStatement(graph)
    }

    def emitExprWrap(e: Expression): String = {
        //    println(e)
        e match {
            case DoPrim(_,_,_,_) | Mux(_,_,_,_) => s"(${emitExpr(e)})"
            case _ => emitExpr(e)
        }
    }

    def emitExpr(e: Expression): String = {
        e match {
            case WRef(name,_,_,_) =>
                if(graph.DetailMap.contains(name))
                    s"(${emitExpr(graph.DetailMap(name))})"
                else
                    name
            case u: UIntLiteral => {
                val maxIn64Bits = (BigInt(1) << 64) - 1
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
            case m: Mux => {
                //println(m)
                val condName = emitExprWrap(m.cond)
                val tvalName = emitExprWrap(m.tval)
                val fvalName = emitExprWrap(m.fval)
                s"$condName ? $tvalName : $fvalName"
            }
            case vif: ValidIf => {
                //println(m)
                val condName = emitExprWrap(vif.cond)
                val tvalName = emitExprWrap(vif.value)
                s"$condName ? $tvalName : 0"
            }
            case w: WSubField => s"${emitExpr(w.expr)}_${w.name}"
            case w: WSubAccess => s"${emitExpr(w.expr)}[${emitExprWrap(w.index)}.as_single_word()]"
            case p: DoPrim =>
//                println(p.op)
//                println(p.args)
                p.op match {
                    case Add => p.args map emitExprWrap mkString(" + ")
                    case Addw=> s"${emitExprWrap(p.args(0))}.addw(${emitExprWrap(p.args(1))})"
                    case Sub => p.args map emitExprWrap mkString(" - ")
                    case Subw=> s"${emitExprWrap(p.args(0))}.subw(${emitExprWrap(p.args(1))})"
                    case Mul => p.args map emitExprWrap mkString(" * ")
                    case Div => p.args map emitExprWrap mkString(" / ")
                    case Rem => p.args map emitExprWrap mkString(" % ")
                    case Lt  => p.args map emitExprWrap mkString(" < ")
                    case Leq => p.args map emitExprWrap mkString(" <= ")
                    case Gt  => p.args map emitExprWrap mkString(" > ")
                    case Geq => p.args map emitExprWrap mkString(" >= ")
                    case Eq  => p.args map emitExprWrap mkString(" == ")
                    case Neq => p.args map emitExprWrap mkString(" != ")
                    case Pad => s"${emitExprWrap(p.args.head)}.pad<${bitWidth(p.tpe)}>()"
                    case AsUInt => s"${emitExprWrap(p.args.head)}.asUInt()"
                    case AsSInt => s"${emitExprWrap(p.args.head)}.asSInt()"
                    case AsClock => throw new Exception("AsClock unimplemented!")
                    case Shl  => s"${emitExprWrap(p.args.head)}.shl<${p.consts.head.toInt}>()"
                    //case Shlw => s"${emitExprWrap(p.args.head)}.shlw<${p.consts.head.toInt}>()"
                    case Shr  => s"${emitExprWrap(p.args.head)}.shr<${p.consts.head.toInt}>()"
                    case Dshl => p.args map emitExprWrap mkString(" << ")
                    case Dshlw => s"${emitExprWrap(p.args(0))}.dshlw(${emitExpr(p.args(1))})"
                    case Dshr => p.args map emitExprWrap mkString(" >> ")
                    case Cvt  => s"${emitExprWrap(p.args.head)}.cvt()"
                    case Neg  => s"-${emitExprWrap(p.args.head)}"
                    case Not  => s"~${emitExprWrap(p.args.head)}"
                    case And  => p.args map emitExprWrap mkString(" & ")
                    case Or   => p.args map emitExprWrap mkString(" | ")
                    case Xor  => p.args map emitExprWrap mkString(" ^ ")
                    case Andr => s"${emitExprWrap(p.args.head)}.andr()"
                    case Orr  => s"${emitExprWrap(p.args.head)}.orr()"
                    case Xorr => s"${emitExprWrap(p.args.head)}.xorr()"
                    case Cat  => s"${emitExprWrap(p.args(0))}.cat(${emitExpr(p.args(1))})"
                    case Bits => s"${emitExprWrap(p.args.head)}.bits<${p.consts(0).toInt},${p.consts(1).toInt}>()"
                    case Head => s"${emitExprWrap(p.args.head)}.head<${p.consts.head.toInt}>()"
                    case Tail => s"${emitExprWrap(p.args.head)}.tail<${p.consts.head.toInt}>()"
                }
            case _ => throw new Exception(s"Don't yet support $e")

        }
    }


}

object Main{

    def main(array: Array[String]) = {
        generate(new File("./firrtl/gcd.fir"))
    }

    def generate(inputFile: File) {
        //val a= new LoweringCompilers
        val circuit = firrtl.Parser.parse(Source.fromFile(inputFile).getLines,
            firrtl.Parser.IgnoreInfo)
        val  topName = circuit.main
        var state = CircuitState(circuit,firrtl.HighForm)
        //println(state.form)

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