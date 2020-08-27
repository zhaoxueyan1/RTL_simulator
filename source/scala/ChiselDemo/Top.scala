package ChiselDemo
import chisel3._

class Top extends Module{
    val io = IO(new Bundle {
        val in_a = Input(UInt(4.W))
        val in_b = Input(UInt(4.W))
        val out = Output(UInt(4.W))
    })
    val t1 = Module(new Passthroughs)
    t1.io.in_a := io.in_a
    t1.io.in_b := io.in_b
    io.out := t1.io.out
}
