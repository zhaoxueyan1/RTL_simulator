;buildInfoPackage: chisel3, version: 3.2-SNAPSHOT, scalaVersion: 2.11.12, sbtVersion: 1.3.2
circuit TestModule :
  module AND :
    input clock : Clock
    input reset : Reset
    output io : {flip io_a : UInt<1>, flip io_b : UInt<1>, io_c : UInt<1>}

    node _T = and(io.io_a, io.io_b) @[AND.scala 12:24]
    io.io_c <= _T @[AND.scala 12:13]

  module Passthroughs :
    input clock : Clock
    input reset : Reset
    output io : {flip in_a : UInt<4>, flip in_b : UInt<4>, out : UInt<4>}

    inst t1 of AND @[Passthroughs.scala 11:20]
    t1.clock <= clock
    t1.reset <= reset
    t1.io.io_a <= io.in_a @[Passthroughs.scala 12:16]
    t1.io.io_b <= io.in_b @[Passthroughs.scala 13:16]
    node _T = and(io.in_a, io.in_b) @[Passthroughs.scala 14:23]
    io.out <= _T @[Passthroughs.scala 14:12]

  module Top :
    input clock : Clock
    input reset : Reset
    output io : {flip in_a : UInt<1>, flip in_b : UInt<1>, out : UInt<1>}

    inst t1 of Passthroughs @[Top.scala 11:20]
    t1.clock <= clock
    t1.reset <= reset
    t1.io.in_a <= io.in_a @[Top.scala 12:16]
    t1.io.in_b <= io.in_b @[Top.scala 13:16]
    node _T = and(io.in_a, io.in_b) @[Top.scala 14:22]
    io.out <= _T @[Top.scala 14:12]

  module TestModule :
    input clock : Clock
    input reset : UInt<1>
    output io : {flip stuff : UInt<1>, flip io_a : UInt<1>, flip io_b : UInt<1>, io_c : UInt<1>}

    reg regClock1 : UInt<1>, clock @[Main.scala 13:28]
    regClock1 <= io.stuff @[Main.scala 13:28]
    inst t of Top @[Main.scala 15:19]
    t.clock <= clock
    t.reset <= reset
    t.io.in_a <= io.io_a @[Main.scala 16:15]
    t.io.in_b <= io.io_b @[Main.scala 17:15]
    node _T = and(io.io_a, io.io_b) @[Main.scala 18:24]
    io.io_c <= _T @[Main.scala 18:13]
    reg regClock2 : UInt<1>, clock @[Main.scala 21:28]
    regClock2 <= io.stuff @[Main.scala 21:28]
    cmem mem : UInt<1>[120] @[Main.scala 22:18]
    write mport _T_1 = mem[UInt<1>("h01")], clock
    _T_1 <= regClock1

