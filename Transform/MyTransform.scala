// See LICENSE for license details.


import java.io.{File, FileWriter}

import essent.Emitter.{emitExpr, emitExprWrap, splatLargeLiteralIntoRawArray}
import firrtl.ir._
import firrtl.{Addw, CircuitState, Dshlw, LowForm, Subw, Transform, Utils, WRef, WSubAccess, WSubField, bitWidth}

import scala.io.Source
import firrtl.Mappers._
import firrtl.PrimOps.{Add, And, Andr, AsClock, AsSInt, AsUInt, Bits, Cat, Cvt, Div, Dshl, Dshr, Eq, Geq, Gt, Head, Leq, Lt, Mul, Neg, Neq, Not, Or, Orr, Pad, Rem, Shl, Shr, Sub, Tail, Xor, Xorr}

import scala.collection.mutable


class Ledger {
  private var moduleName: Option[String] = None
  private val modules = mutable.Set[String]()
  private val moduleMuxMap = mutable.Map[String, Int]()
  def foundMux(): Unit = moduleName match {
    case None => sys.error("Module name not defined in Ledger!")
    case Some(name) => moduleMuxMap(name) = moduleMuxMap.getOrElse(name, 0) + 1
  }
  def getModuleName: String = moduleName match {
    case None => Utils.error("Module name not defined in Ledger!")
    case Some(name) => name
  }
  def setModuleName(myName: String): Unit = {
    modules += myName
    moduleName = Some(myName)
  }
  def serialize: String = {
    modules map { myName =>
      s"$myName => ${moduleMuxMap.getOrElse(myName, 0)} muxes!"
    } mkString "\n"
  }
}


class AnalyzeCircuit extends Transform {
  def inputForm = LowForm
  def outputForm = LowForm

  def execute(state: CircuitState): CircuitState = {
    val ledger = new Ledger()
    val circuit = state.circuit

    circuit map walkModule(ledger)

    state
  }

  def walkModule(ledger: Ledger)(m: DefModule): DefModule = {
    ledger.setModuleName(m.name)
    m match {
      case Module(info, name, ports, body) =>
        m
      case ExtModule(info, name, ports, defname, params)=>
        m
    }
    m map walkStatement(ledger)
  }

  def walkStatement(ledger: Ledger)(s: Statement): Statement = {

    s map walkExpression(ledger)

    s map walkStatement(ledger)

    s match {
      case DefRegister(info, name, tpe, clock, reset, init)=>
        println(name)
        println(tpe)
        s
      case DefNode(info, name, value)=>
        println(name)
        println(value)
        s
      case DefWire(info, name, value)=>
        println(name)
        println(value)
        s
      case Stop(info, ret, clk, en)=>
        s
      case Conditionally(info, pred, conseq, alt)=>
        println(pred)
        println(conseq)
        s
      case m : DefMemory =>
        s
      case ins : DefInstance =>
        s
      case con : Connect =>
        s
      case parCon : PartialConnect =>
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
  }

  def walkExpression(ledger: Ledger)(e: Expression): Expression = {
    case w: WRef => w.name
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
      val condName = emitExprWrap(m.cond)
      val tvalName = emitExprWrap(m.tval)
      val fvalName = emitExprWrap(m.fval)
      s"$condName ? $tvalName : $fvalName"
    }
    case w: WSubField => s"${emitExpr(w.expr)}.${w.name}"
    case w: WSubAccess => s"${emitExpr(w.expr)}[${emitExprWrap(w.index)}.as_single_word()]"
    case p: DoPrim => p.op match {
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

    e map walkExpression(ledger)

  }
}

object Main{
  def main(array: Array[String]) = {
    generate(new File("./firrtl/Top.fir"))
  }
  def generate(inputFile: File) {
    val circuit = firrtl.Parser.parse(Source.fromFile(inputFile).getLines,
      firrtl.Parser.IgnoreInfo)
    val  topName = circuit.main
    var analyzeCircuit = new AnalyzeCircuit()
    analyzeCircuit.execute(CircuitState(circuit, firrtl.ChirrtlForm))
  }
}