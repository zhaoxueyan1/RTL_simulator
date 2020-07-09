package DiGraph
import java.io.{File, FileWriter}

import firrtl.ir._
import firrtl.{Addw, ChirrtlToHighFirrtl, CircuitState, Dshlw, HighFirrtlToMiddleFirrtl, IRToWorkingIR, InstanceKind, LowFirrtlOptimization, LowForm, MiddleFirrtlToLowFirrtl, ResolveAndCheck, Subw, Transform, Utils, WDefInstance, WRef, WSubAccess, WSubField, bitWidth}

import scala.io.Source
import firrtl.Mappers._
import firrtl.PrimOps.{Add, And, Andr, AsClock, AsSInt, AsUInt, Bits, Cat, Cvt, Div, Dshl, Dshr, Eq, Geq, Gt, Head, Leq, Lt, Mul, Neg, Neq, Not, Or, Orr, Pad, Rem, Shl, Shr, Sub, Tail, Xor, Xorr}
import firrtl.analyses.InstanceGraph
import firrtl.annotations.{ModuleTarget, ReferenceTarget}
import firrtl.passes.MemPortUtils
import firrtl.stage.{Forms, TransformManager}

import scala.collection.mutable

class AnalyzeCircuit(val circuit: Circuit) extends Transform {
    def inputForm = LowForm

    def outputForm = LowForm

    val graph = new Graph()
    var flag = false
    val instanceGraph = new InstanceGraph(circuit)

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

    def execute(circuitState: CircuitState): CircuitState = {
        circuit.modules.foreach(e=>{
            if(circuit.main == e.name) {
                graph.InstanceNode:+=(e.name,e.name)
                graph.instanceCnt+= 1
                walkModule(graph.instanceCnt-1)("")(e)
            } else
                graph.moduleMap+=(e.name->e)
        })

        graph.Output()
        circuitState
    }

    def walkModule(instanceID: Int)(suffix: String)(m: DefModule): DefModule = {

        m match {
            case Module(info, name, ports, body) =>
                ports.foreach(e => {
                    val tuple = Main.genCppType(e.tpe)
                    graph.addNode(instanceID,e.name,tuple._1,tuple._2,e.direction.toString,"Port","Node")

                })
            case ExtModule(info, name, ports, defname, params) =>
                ports.foreach(e => {
                    val tuple = Main.genCppType(e.tpe)
                    graph.addNode(instanceID,e.name,tuple._1,tuple._2,e.direction.toString,"Port","Node")

                })
        }
        m map walkStatement(instanceID)(suffix)
    }

//    def buildMemory(mt: ModuleTarget, d: DefMemory): Unit = {
//        val readers = d.readers.toSet
//        val readwriters = d.readwriters.toSet
//        val mem = mt.ref(d.name)
//        MemPortUtils.memType(d).fields.foreach {
//            case Field(name, _, _: BundleType) if readers.contains(name) || readwriters.contains(name) =>
//                val port = mem.field(name)
//                val sources = Seq(
//                    port.field("clk"),
//                    port.field("en"),
//                    port.field("addr")
//                ) ++ (if (readwriters.contains(name)) Seq(port.field("wmode")) else Nil)
//
//                val data = if (readers.contains(name)) port.field("data") else port.field("rdata")
//                val sinks = data.leafSubTargets(d.dataType)
//
//                sources.foreach {
//                    mdg.addVertex
//                }
//                sinks.foreach { sink =>
//                    mdg.addVertex(sink)
//                    sources.foreach { source => mdg.addEdge(source, sink) }
//                }
//            case _ =>
//        }
//    }

    def walkStatement(instanceID: Int)(suffix: String)(s: Statement): Statement = {
//        println(s)
        s match {
            case r: DefRegister =>
//                println(r)
                val tuple = Main.genCppType(r.tpe)
                graph.addNode(instanceID,r.name,tuple._1,tuple._2,"None","Reg","None")

            case DefNode(info, name, value) =>
                val t = parseExpr(instanceID)(suffix)(value)
                val t2 = graph.addNode(instanceID,name,"12",32,"None","Node","None")

                t2.cppType = t.cppType
                t2.cppWidth = t.cppWidth
                graph.EdgeSet :+= new Edge((instanceID,name), (t.instanceID,t.name))

            case DefWire(info, name, tpe) =>
                val tuple = Main.genCppType(tpe)
                graph.addNode(instanceID,name,tuple._1,tuple._2,"None","wire","None")

            case Stop(info, ret, clk, en) =>
            case Conditionally(info, pred, conseq, alt) =>
            case m: DefMemory =>
                val readers = m.readers.toSet
                val readwriters = m.readwriters.toSet
                val portUtils = MemPortUtils.memType(m)
                portUtils.fields.foreach {
                    field: Field =>
                        var expr = 0
                        if (readers.contains(name))
                            expr |= 1
                        if (readwriters.contains(name))
                            expr |= 2
                        field.tpe match {
                            case bundleType: BundleType =>
                                bundleType.fields.foreach(e => {
                                    val tuple = Main.genCppType(e.tpe)
                                    graph.addNode(instanceID,m.name + field.name + e.name, tuple._1, tuple._2, e.flip.toString, s"Mem${e.name}", s"$expr")
                                })
                            case _ =>
                        }
                }

            case instance: WDefInstance =>
                //                println(instance.module)
                graph.InstanceLinkMP+=((instanceID,instance.name)->graph.instanceCnt)
                graph.InstanceNode:+=(instance.name,instance.module)
                graph.InstanceGraph+=(graph.instanceCnt->instanceID)
                graph.instanceCnt+=1
//                println(graph.InstanceID,instance.name)
                walkModule(graph.instanceCnt-1)(suffix)(graph.moduleMap(instance.module))

            case connect: Connect => //等式左边
                val node2 = parseExpr(instanceID)(suffix)(connect.expr)
                val name  = parseExpr(instanceID)(suffix)(connect.loc)
                graph.EdgeSet:+=new Edge((name.instanceID,name.name),(node2.instanceID,node2.name))

            case partialConnect: PartialConnect =>
            case invalid: IsInvalid =>
            case attach: Attach =>
            case stop: DefInstance =>
            case p: Print =>
            //                val formatters = "(%h)|(%x)|(%d)|(%ld)".r.findAllIn(p.string.serialize).toList
            //                val argWidths = p.args map { e: Expression => bitWidth(e.tpe) }
            //                if (!(argWidths forall {
            //                    _ <= 64
            //                })) throw new Exception(s"Can't print wide signals")
            //                val replacements = formatters zip argWidths map { case (format, width) =>
            //                    if (format == "%h" || format == "%x") {
            //                        val printWidth = math.ceil((width / 4).toDouble).toInt
            //                        (format, s"""%0${printWidth}" PRIx64 """")
            //                    } else {
            //                        val printWidth = math.ceil(math.log10((1l << width.toInt).toDouble)).toInt
            //                        (format, s"""%${printWidth}" PRIu64 """")
            //                    }
            //                }
            //                val formatString = replacements.foldLeft(p.string.serialize) {
            //                    case (str, (searchFor, replaceWith)) => str.replaceFirst(searchFor, replaceWith)
            //                }
            //                val printfArgs = Seq(s""""$formatString"""") ++
            //                  (p.args map { arg => s"${emitExpr(arg)}.as_single_word()" })
            //                Seq(s"if (UNLIKELY(done_reset && update_registers && verbose && ${emitExpr(p.en)})) printf(${printfArgs mkString (", ")});")
            case _ =>
        }
        s map walkStatement(instanceID)(suffix)
    }


    def parseExpr(instanceID: Int)(suffix: String)(e: Expression): Node = {
        e match {
            case WRef(name, tpe, kind, _) =>
                if(kind==InstanceKind)
                    graph.NodeMP((graph.InstanceLinkMP((instanceID,name)),suffix))
                else
                    graph.NodeMP((instanceID,name+suffix))
            case u: UIntLiteral => {
                val maxIn64Bits = (BigInt(1) << 64) - 1
                val width = bitWidth(u.tpe)
                val asHexStr = u.value.toString(16)
                var res = "1"
                if ((width <= 64) || (u.value <= maxIn64Bits)) res = s"UInt<$width>(0x$asHexStr)"
                else res = s"UInt<$width>(${splatLargeLiteralIntoRawArray(u.value, width)})"
                graph.addNode(instanceID,s"Uint${graph.cnt}",s"UInt",width,"None","UIntLiteral",res)
            }
            case u: SIntLiteral => {
                val width = bitWidth(u.tpe)
                var res = "1"
                if (width <= 64) res = s"SInt<$width>(${u.value.toString(10)})"
                else res = s"SInt<$width>(${splatLargeLiteralIntoRawArray(u.value, width)})"
                graph.addNode(instanceID,s"Sint${graph.cnt}",s"SInt",width,"None","SIntLiteral",res)
            }
            case m: Mux => {
                //println(m)
                val condNode = parseExpr(instanceID)(suffix)(m.cond)
                val tvalNode = parseExpr(instanceID)(suffix)(m.tval)
                val fvalNode = parseExpr(instanceID)(suffix)(m.fval)
                val t = graph.addNode(instanceID,s"Mux${graph.cnt}","Mux",tvalNode.cppWidth,"None","Mux",s"${condNode.name} ? ${tvalNode.name} : ${fvalNode.name}")
                graph.EdgeSet:+=new Edge((t.instanceID,t.name) , (condNode.instanceID,condNode.name))
                graph.EdgeSet:+=new Edge((t.instanceID,t.name) , (tvalNode.instanceID,tvalNode.name))
                graph.EdgeSet:+=new Edge((t.instanceID,t.name) , (fvalNode.instanceID,fvalNode.name))
                t
            }
            case vif: ValidIf => {
                //println(m)
                val condNode = parseExpr(instanceID)(suffix)(vif.cond)
                val tvalNode = parseExpr(instanceID)(suffix)(vif.value)
                val t = graph.addNode(instanceID,s"ValidIf${graph.cnt}","ValidIf",tvalNode.cppWidth,"None","ValidIf",s"${condNode.name} ? ${tvalNode.name}")
                graph.EdgeSet:+=new Edge((t.instanceID,t.name) ,(condNode.instanceID,condNode.name) )
                graph.EdgeSet:+=new Edge((t.instanceID,t.name) ,(tvalNode.instanceID,tvalNode.name) )
                t
            }
            case w: WSubField =>
//                println(w)
                parseExpr(instanceID)(w.name+suffix)(w.expr)
            case w: WSubAccess =>
                val tuple = Main.genCppType(w.tpe)
                val t = graph.addNode(instanceID,s"WSubAccess${graph.cnt}",tuple._1,tuple._2,"None","WSubAccess",s"${emitExpr(w.index)}")
                val t2= parseExpr(instanceID)(suffix)(w.expr)
                graph.EdgeSet:+=new Edge((t.instanceID,t.name),(t2.instanceID,t2.name))
                //                s"${emitExpr(w.expr)}[${emitExpr(w.index)}.as_single_word()]"
                t

            case p: DoPrim =>
                p.op match {
                    case Add =>
                        val t = graph.addNode(instanceID,s"Add${graph.cnt}","None",0,"None","Add","None");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Addw =>
                        val t = graph.addNode(instanceID,s"Add${graph.cnt}","None",0,"None","Addw","None");
                        val l = parseExpr(instanceID)(suffix)(p.args(0))
                        val r = parseExpr(instanceID)(suffix)(p.args(1))
                        t.cppType = l.cppType
                        t.cppWidth = l.cppWidth
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(l.instanceID,l.name))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(r.instanceID,r.name))
                        graph.EdgeSet:+= new Edge((l.instanceID,l.name),(t.instanceID,t.name))
                        t
                    case Sub =>
                        val t = graph.addNode(instanceID,s"Sub${graph.cnt}","None",0,"None","Sub","None");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Subw => val t = graph.addNode(instanceID,s"Add${graph.cnt}","None",0,"None","Subw","None");
                        val l = parseExpr(instanceID)(suffix)(p.args(0))
                        val r = parseExpr(instanceID)(suffix)(p.args(1))
                        t.cppType = l.cppType
                        t.cppWidth = l.cppWidth
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(l.instanceID,l.name))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(r.instanceID,r.name))
                        graph.EdgeSet:+= new Edge((l.instanceID,l.name),(t.instanceID,t.name))
                        t

                    case Mul => val t = graph.addNode(instanceID,s"Mul${graph.cnt}","None",0,"None","Mul","*");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Div => val t = graph.addNode(instanceID,s"Div${graph.cnt}","None",0,"None","Div","/");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Rem => val t = graph.addNode(instanceID,s"Rem${graph.cnt}","None",Integer.MAX_VALUE,"None","Rem","%");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                            t.cppWidth = e.cppWidth.min(t.cppWidth)
                            t.cppType = e.cppType
                        })
                        t
                    case Lt =>
                        val t = graph.addNode(instanceID,s"Lt${graph.cnt}","bool",1,"None","Lt","<");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                        })
                        t
                    case Leq => val t = graph.addNode(instanceID,s"Leq${graph.cnt}","bool",1,"None","Leq","<=");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                        })
                        t
                    case Gt =>   val t = graph.addNode(instanceID,s"Gt${graph.cnt}","bool",1,"None","Gt",">");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                        })
                        t
                    case Geq => val t = graph.addNode(instanceID,s"Geq${graph.cnt}","bool",1,"None","Geq",">=");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                        })
                        t
                    case Eq =>  val t = graph.addNode(instanceID,s"Eq${graph.cnt}","bool",1,"None","Eq","==");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                        })
                        t
                    case Neq => val t = graph.addNode(instanceID,s"Neq${graph.cnt}","bool",1,"None","Neq","!=");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                        })
                        t
                    case Pad =>
                        val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Pad${graph.cnt}",node.cppType,bitWidth(p.tpe),"None","Pad","Pad");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    //s"${emitExpr(p.args.head)}.pad<${}>()"
                    case AsUInt => val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"AsUInt${graph.cnt}",s"UInt",bitWidth(p.tpe),"None","AsUInt","AsUInt");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case AsSInt => val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"AsSInt${graph.cnt}",s"SInt",bitWidth(p.tpe),"None","AsSInt","AsSInt");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case AsClock => throw new Exception("AsClock unimplemented!")
                    case Shl =>
                        val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Shl${graph.cnt}",node.cppType,bitWidth(p.tpe)+p.consts.head.toInt,"None","Shl",s"${p.consts.head.toInt}");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    //case Shlw => s"${emitExpr(p.args.head)}.shlw<${p.consts.head.toInt}>()"
                    case Shr =>
                        val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Shr${graph.cnt}",node.cppType,bitWidth(p.tpe)-p.consts.head.toInt,"None","Shr",s"${p.consts.head.toInt}");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case Dshl =>
                        val t1 = parseExpr(instanceID)(suffix)(p.args(0))
                        val t2 = parseExpr(instanceID)(suffix)(p.args(1))
                        val _2 = BigInt(2)
                        val t = graph.addNode(instanceID,s"Dshl${graph.cnt}",s"${t1.cppType}",t1.cppWidth+_2.pow(t2.cppWidth.toInt)-1,"None","Dshl","Dshl");
                        graph.EdgeSet:+=new Edge((t.instanceID,t.name),(t.instanceID,t1.name))
                        graph.EdgeSet:+=new Edge((t.instanceID,t.name),(t2.instanceID,t2.name))
                        t
                    case Dshr => val t1 = parseExpr(instanceID)(suffix)(p.args(0))
                        val t2 = parseExpr(instanceID)(suffix)(p.args(1))
                        val t = graph.addNode(instanceID,s"Dshr${graph.cnt}",s"${t1.cppType}",t1.cppWidth,"None","Dshr","Dshr");
                        graph.EdgeSet:+=new Edge((t.instanceID,t.name),(t.instanceID,t1.name))
                        graph.EdgeSet:+=new Edge((t.instanceID,t.name),(t2.instanceID,t2.name))
                        t
                    case Dshlw => val t1 = parseExpr(instanceID)(suffix)(p.args(0))
                        val t2 = parseExpr(instanceID)(suffix)(p.args(1))
                        val _2 = BigInt(2)
                        val t = graph.addNode(instanceID,s"Dshlw${graph.cnt}",s"${t1.cppType}",t1.cppWidth+_2.pow(t2.cppWidth.toInt)-1,"None","Dshlw","Dshlw");
                        graph.EdgeSet:+=new Edge((t.instanceID,t.name),(t.instanceID,t1.name))
                        graph.EdgeSet:+=new Edge((t.instanceID,t.name),(t2.instanceID,t2.name))
                        graph.EdgeSet:+=new Edge((t1.instanceID,t1.name),(t.instanceID,t.name))
                        t
                    case Cvt => val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Cvt${graph.cnt}",s"SInt",node.cppWidth,"None","Cvt","Cvt");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case Neg => val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Neg${graph.cnt}",s"SInt",bitWidth(p.tpe),"None","Neg","-");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case Not => val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Not${graph.cnt}",s"bool",1,"None","Not","!");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case And => val t = graph.addNode(instanceID,s"And${graph.cnt}","None",0,"None","And","&");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Or =>  val t = graph.addNode(instanceID,s"Or${graph.cnt}","None",0,"None","Or","|");
                        p.args map parseExpr(instanceID)(suffix) foreach(e => {
                            graph.EdgeSet:+= new Edge((t.instanceID,t.name),(e.instanceID,e.name))
                            t.cppWidth = e.cppWidth
                            t.cppType = e.cppType
                        })
                        t
                    case Xor => val t = graph.addNode(instanceID,s"Xor${graph.cnt}","None",0,"None","Xor","^");
                        val l = parseExpr(instanceID)(suffix)(p.args(0))
                        val r = parseExpr(instanceID)(suffix)(p.args(1))
                        t.cppType = l.cppType
                        t.cppWidth = l.cppWidth
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(l.instanceID,l.name))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(r.instanceID,r.name))
                        t
                    case Andr => val t = graph.addNode(instanceID,s"Andr${graph.cnt}","bool",1,"None","Andr","&&");
                        val l = parseExpr(instanceID)(suffix)(p.args(0))
                        val r = parseExpr(instanceID)(suffix)(p.args(1))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(l.instanceID,l.name))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(r.instanceID,r.name))
                        t
                    case Orr =>  val t = graph.addNode(instanceID,s"Orr${graph.cnt}","bool",1,"None","Orr","||");
                        val l = parseExpr(instanceID)(suffix)(p.args(0))
                        val r = parseExpr(instanceID)(suffix)(p.args(1))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(l.instanceID,l.name))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(r.instanceID,r.name))
                        t
                    case Xorr =>val t = graph.addNode(instanceID,s"Xorr${graph.cnt}","bool",1,"None","Xorr","Xorr");
                        val l = parseExpr(instanceID)(suffix)(p.args(0))
                        val r = parseExpr(instanceID)(suffix)(p.args(1))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(l.instanceID,l.name))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(r.instanceID,r.name))
                        t
                    case Cat =>
                        val l = parseExpr(instanceID)(suffix)(p.args(0))
                        val r = parseExpr(instanceID)(suffix)(p.args(1))
                        val t = graph.addNode(instanceID,s"Cat${graph.cnt}",l.cppType,l.cppWidth+r.cppWidth,"None","Cat","Cat");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(l.instanceID,l.name))
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(r.instanceID,r.name))
                        t
                    case Bits =>
                        val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Bits${graph.cnt}",s"${node.cppType}",p.consts(0).toInt-p.consts(1)+1.toInt,s"${p.consts(0).toInt}","Bits",s"${(p.consts(0),p.consts(1))}");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case Head =>
                        val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Head${graph.cnt}",s"${node.cppType}",p.consts.head.toInt,"None","Head",s"${p.consts.head.toInt}");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                    case Tail =>
                        val node = parseExpr(instanceID)(suffix)(p.args.head)
                        val t = graph.addNode(instanceID,s"Tail${graph.cnt}",s"${node.cppType}",node.cppWidth-p.consts.head.toInt,"None","Tail",s"${p.consts.head.toInt}");
                        graph.EdgeSet:+= new Edge((t.instanceID,t.name),(node.instanceID,node.name))
                        t
                }
            case _ => throw new Exception(s"Don't yet support $e")

        }
    }

    //    def emitExpr(e: Expression): String = e match {
    //        case DoPrim(_, _, _, _) | Mux(_, _, _, _) => s"(${emitExpr(e)})"
    //        case _ => emitExpr(e)
    //    }

    def emitExpr(e: Expression,instanceID: String=""): String = {
        e match {
            case WRef(name, tpe, _, _) =>
                instanceID+name
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
            case w: WSubField => instanceID+s"${emitExpr(w.expr)}${w.name}"
            case w: WSubAccess => instanceID+s"${emitExpr(w.expr)}[${w.index}]"
            case _ => throw new Exception(s"Don't yet support $e")

        }
    }
}