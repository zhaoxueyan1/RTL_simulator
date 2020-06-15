circuit GCD :
  module GCD :
    input clock : Clock
    input reset : UInt<1>
    output io : {flip a : UInt<32>, flip b : UInt<32>, flip e : UInt<1>, z : UInt<32>, v : UInt<1>}
    reg x : UInt<32>, clock
    reg y : UInt<32>, clock
    node _T_14 = gt(x, y)
    when _T_14 :
      node _T_15 = sub(x, y)
      node _T_16 = tail(_T_15, 1)
      x <= _T_16
    node _T_18 = eq(_T_14, UInt<1>("h00"))
    when _T_18 :
      node _T_19 = sub(y, x)
      node _T_20 = tail(_T_19, 1)
      y <= _T_20
    when io.e :
      x <= io.a
      y <= io.b
    io.z <= x
    node _T_22 = eq(y, UInt<1>("h00"))
    io.v <= _T_22
