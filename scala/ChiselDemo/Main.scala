package ChiselDemo

import chisel3._

object Main extends App{
    Driver.execute(args,()=>new Top)
}