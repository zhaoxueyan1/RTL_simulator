package DiGraph

import java.io.{File, FileWriter}

import firrtl.ir._
import firrtl.{Addw, ChirrtlToHighFirrtl, CircuitState, Dshlw, HighFirrtlToMiddleFirrtl, IRToWorkingIR, LowFirrtlOptimization, LowForm, MiddleFirrtlToLowFirrtl, ResolveAndCheck, Subw, Transform, Utils, WDefInstance, WRef, WSubAccess, WSubField, bitWidth}

import scala.io.Source
import firrtl.Mappers._
import firrtl.PrimOps.{Add, And, Andr, AsClock, AsSInt, AsUInt, Bits, Cat, Cvt, Div, Dshl, Dshr, Eq, Geq, Gt, Head, Leq, Lt, Mul, Neg, Neq, Not, Or, Orr, Pad, Rem, Shl, Shr, Sub, Tail, Xor, Xorr}
import firrtl.stage.{Forms, TransformManager}

import scala.collection.mutable




object Main{
    def genCppType(tpe: Type):(String,BigInt) = tpe match {
        case UIntType(IntWidth(w)) => (s"UInt",w)
        case SIntType(IntWidth(w)) => (s"SInt",w)
        case ClockType => (s"ClockType",1)
        case _ => throw new Exception(s"No CPP type implemented for $tpe")
    }

    def main(array: Array[String]) = {
        generate(new File("./Top.fir"))
    }

    def generate(inputFile: File) {
        val circuit = firrtl.Parser.parse(Source.fromFile(inputFile).getLines,
            firrtl.Parser.IgnoreInfo)
        val  topName = circuit.main
        var state = CircuitState(circuit,firrtl.HighForm)

        val ChirrtlToWorkingIR = new TransformManager(Forms.MinimalHighForm, Forms.ChirrtlForm)
        val irToWorkingIR = new TransformManager(Forms.WorkingIR, Forms.MinimalHighForm)
        state = ChirrtlToWorkingIR.execute(state)
        state = irToWorkingIR.execute(state)
        val compiler = new firrtl.stage.transforms.Compiler(
            targets=firrtl.stage.Forms.LowForm,
            currentState=firrtl.stage.Forms.MinimalHighForm
        )
        state = compiler.transform(state)
        val analyzeCircuit = new AnalyzeCircuit(state.circuit)
        analyzeCircuit.execute(state)
    }
}