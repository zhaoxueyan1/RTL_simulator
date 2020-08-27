package ChiselDemo
import chisel3._

class AND extends RawModule {
    def Xor(a: UInt, b: UInt) = a+b
    val io = IO(new Bundle {
        val a = Input(UInt(1.W))
        val b = Input(UInt(1.W))
        val c = Output(UInt(1.W))
    })

    io.c := Xor(io.a,io.b)
}