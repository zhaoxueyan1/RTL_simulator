;buildInfoPackage: chisel3, version: 3.2-SNAPSHOT, scalaVersion: 2.11.12, sbtVersion: 1.3.2
circuit Top : 
  module AND : 
    output io : {flip a : UInt<1>, flip b : UInt<1>, c : UInt<1>}
    
    node _T = add(io.a, io.b) @[FullAdder.scala 9:34]
    node _T_1 = tail(_T, 1) @[FullAdder.scala 9:34]
    io.c <= _T_1 @[FullAdder.scala 16:10]
    
  module Passthroughs : 
    input clock : Clock
    input reset : Reset
    output io : {flip in_a : UInt<4>, flip in_b : UInt<4>, out : UInt<4>}
    
    inst t1 of AND @[FullAdder.scala 24:20]
    t1.io.a <= io.in_a @[FullAdder.scala 25:13]
    t1.io.b <= io.in_b @[FullAdder.scala 26:13]
    io.out <= t1.io.c @[FullAdder.scala 27:12]
    
  module Top : 
    input clock : Clock
    input reset : UInt<1>
    output io : {flip in_a : UInt<4>, flip in_b : UInt<4>, out : UInt<4>}
    
    inst t1 of Passthroughs @[FullAdder.scala 35:20]
    t1.clock <= clock
    t1.reset <= reset
    t1.io.in_a <= io.in_a @[FullAdder.scala 36:16]
    t1.io.in_b <= io.in_b @[FullAdder.scala 37:16]
    io.out <= t1.io.out @[FullAdder.scala 38:12]
    
