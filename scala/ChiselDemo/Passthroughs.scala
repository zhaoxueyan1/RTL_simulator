package ChiselDemo
import chisel3._

class Passthroughs extends Module {
    val io = IO(new Bundle {
        val in_a = Input(UInt(4.W))
        val in_b = Input(UInt(4.W))
        val out = Output(UInt(4.W))
    })
    val t1 = Module(new AND)
    t1.io.a := io.in_a
    t1.io.b := io.in_b
    io.out := t1.io.c
}