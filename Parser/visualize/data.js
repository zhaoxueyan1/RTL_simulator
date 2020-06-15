let data = {
  "nodes": [
    {
      "label": "~GCD|GCD>clock",
      "description": "Reference(clock,ClockType,PortKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>reset",
      "description": "Reference(reset,UIntType(IntWidth(1)),PortKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>io",
      "description": "Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow)"
    },
    {
      "label": "~GCD|GCD>x",
      "description": "Reference(x,UIntType(IntWidth(32)),RegKind,DuplexFlow)"
    },
    {
      "label": "~GCD|GCD>x@clock",
      "description": "Reference(clock,ClockType,PortKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>x@reset",
      "description": "UIntLiteral(0,IntWidth(1))"
    },
    {
      "label": "~GCD|GCD>@0#0",
      "description": "UIntLiteral(0,IntWidth(1))"
    },
    {
      "label": "~GCD|GCD>x@init",
      "description": "Reference(x,UIntType(IntWidth(32)),RegKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>y",
      "description": "Reference(y,UIntType(IntWidth(32)),RegKind,DuplexFlow)"
    },
    {
      "label": "~GCD|GCD>y@clock",
      "description": "Reference(clock,ClockType,PortKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>y@reset",
      "description": "UIntLiteral(0,IntWidth(1))"
    },
    {
      "label": "~GCD|GCD>@0#1",
      "description": "UIntLiteral(0,IntWidth(1))"
    },
    {
      "label": "~GCD|GCD>y@init",
      "description": "Reference(y,UIntType(IntWidth(32)),RegKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>_T_14",
      "description": "Reference(_T_14,UIntType(IntWidth(1)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@gt#0",
      "description": "DoPrim(gt,ArrayBuffer(Reference(x,UIntType(IntWidth(32)),RegKind,SourceFlow), Reference(y,UIntType(IntWidth(32)),RegKind,SourceFlow)),ArrayBuffer(),UIntType(IntWidth(1)))"
    },
    {
      "label": "~GCD|GCD>_T_15",
      "description": "Reference(_T_15,UIntType(IntWidth(33)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@sub#0",
      "description": "DoPrim(sub,ArrayBuffer(Reference(x,UIntType(IntWidth(32)),RegKind,SourceFlow), Reference(y,UIntType(IntWidth(32)),RegKind,SourceFlow)),ArrayBuffer(),UIntType(IntWidth(33)))"
    },
    {
      "label": "~GCD|GCD>_T_16",
      "description": "Reference(_T_16,UIntType(IntWidth(32)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@tail#0",
      "description": "DoPrim(tail,ArrayBuffer(Reference(_T_15,UIntType(IntWidth(33)),NodeKind,SourceFlow)),ArrayBuffer(1),UIntType(IntWidth(32)))"
    },
    {
      "label": "~GCD|GCD>_GEN_0",
      "description": "Reference(_GEN_0,UIntType(IntWidth(32)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@mux#0",
      "description": "Mux(Reference(_T_14,UIntType(IntWidth(1)),NodeKind,SourceFlow),Reference(_T_16,UIntType(IntWidth(32)),NodeKind,SourceFlow),Reference(x,UIntType(IntWidth(32)),ExpKind,DuplexFlow),UIntType(IntWidth(32)))"
    },
    {
      "label": "~GCD|GCD>_T_18",
      "description": "Reference(_T_18,UIntType(IntWidth(1)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@eq#0",
      "description": "DoPrim(eq,ArrayBuffer(Reference(_T_14,UIntType(IntWidth(1)),NodeKind,SourceFlow), UIntLiteral(0,IntWidth(1))),ArrayBuffer(),UIntType(IntWidth(1)))"
    },
    {
      "label": "~GCD|GCD>@0#2",
      "description": "UIntLiteral(0,IntWidth(1))"
    },
    {
      "label": "~GCD|GCD>_T_19",
      "description": "Reference(_T_19,UIntType(IntWidth(33)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@sub#1",
      "description": "DoPrim(sub,ArrayBuffer(Reference(y,UIntType(IntWidth(32)),RegKind,SourceFlow), Reference(x,UIntType(IntWidth(32)),RegKind,SourceFlow)),ArrayBuffer(),UIntType(IntWidth(33)))"
    },
    {
      "label": "~GCD|GCD>_T_20",
      "description": "Reference(_T_20,UIntType(IntWidth(32)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@tail#1",
      "description": "DoPrim(tail,ArrayBuffer(Reference(_T_19,UIntType(IntWidth(33)),NodeKind,SourceFlow)),ArrayBuffer(1),UIntType(IntWidth(32)))"
    },
    {
      "label": "~GCD|GCD>_GEN_1",
      "description": "Reference(_GEN_1,UIntType(IntWidth(32)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@mux#1",
      "description": "Mux(Reference(_T_18,UIntType(IntWidth(1)),NodeKind,SourceFlow),Reference(_T_20,UIntType(IntWidth(32)),NodeKind,SourceFlow),Reference(y,UIntType(IntWidth(32)),ExpKind,DuplexFlow),UIntType(IntWidth(32)))"
    },
    {
      "label": "~GCD|GCD>_GEN_2",
      "description": "Reference(_GEN_2,UIntType(IntWidth(32)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@mux#2",
      "description": "Mux(SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),e,UIntType(IntWidth(1)),SourceFlow),SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),a,UIntType(IntWidth(32)),SourceFlow),Reference(_GEN_0,UIntType(IntWidth(32)),NodeKind,SourceFlow),UIntType(IntWidth(32)))"
    },
    {
      "label": "~GCD|GCD>io.e",
      "description": "SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),e,UIntType(IntWidth(1)),SourceFlow)"
    },
    {
      "label": "~GCD|GCD>io.a",
      "description": "SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),a,UIntType(IntWidth(32)),SourceFlow)"
    },
    {
      "label": "~GCD|GCD>_GEN_3",
      "description": "Reference(_GEN_3,UIntType(IntWidth(32)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@mux#3",
      "description": "Mux(SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),e,UIntType(IntWidth(1)),SourceFlow),SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),b,UIntType(IntWidth(32)),SourceFlow),Reference(_GEN_1,UIntType(IntWidth(32)),NodeKind,SourceFlow),UIntType(IntWidth(32)))"
    },
    {
      "label": "~GCD|GCD>io.b",
      "description": "SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),b,UIntType(IntWidth(32)),SourceFlow)"
    },
    {
      "label": "~GCD|GCD>_T_22",
      "description": "Reference(_T_22,UIntType(IntWidth(1)),ExpKind,SourceFlow)"
    },
    {
      "label": "~GCD|GCD>@eq#1",
      "description": "DoPrim(eq,ArrayBuffer(Reference(y,UIntType(IntWidth(32)),RegKind,SourceFlow), UIntLiteral(0,IntWidth(1))),ArrayBuffer(),UIntType(IntWidth(1)))"
    },
    {
      "label": "~GCD|GCD>@0#3",
      "description": "UIntLiteral(0,IntWidth(1))"
    },
    {
      "label": "~GCD|GCD>io.z",
      "description": "SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),z,UIntType(IntWidth(32)),SinkFlow)"
    },
    {
      "label": "~GCD|GCD>io.v",
      "description": "SubField(Reference(io,BundleType(ArrayBuffer(Field(a,Flip,UIntType(IntWidth(32))), Field(b,Flip,UIntType(IntWidth(32))), Field(e,Flip,UIntType(IntWidth(1))), Field(z,Default,UIntType(IntWidth(32))), Field(v,Default,UIntType(IntWidth(1))))),PortKind,SinkFlow),v,UIntType(IntWidth(1)),SinkFlow)"
    }
  ],
  "edges": [
    {
      "source": "~GCD|GCD>clock",
      "target": "~GCD|GCD>x@clock",
      "label": "0"
    },
    {
      "source": "~GCD|GCD>clock",
      "target": "~GCD|GCD>y@clock",
      "label": "1"
    },
    {
      "source": "~GCD|GCD>x",
      "target": "~GCD|GCD>x@init",
      "label": "2"
    },
    {
      "source": "~GCD|GCD>x",
      "target": "~GCD|GCD>@gt#0",
      "label": "3"
    },
    {
      "source": "~GCD|GCD>x",
      "target": "~GCD|GCD>@sub#0",
      "label": "4"
    },
    {
      "source": "~GCD|GCD>x",
      "target": "~GCD|GCD>@mux#0",
      "label": "5"
    },
    {
      "source": "~GCD|GCD>x",
      "target": "~GCD|GCD>@sub#1",
      "label": "6"
    },
    {
      "source": "~GCD|GCD>x",
      "target": "~GCD|GCD>io.z",
      "label": "7"
    },
    {
      "source": "~GCD|GCD>x@clock",
      "target": "~GCD|GCD>x",
      "label": "8"
    },
    {
      "source": "~GCD|GCD>x@reset",
      "target": "~GCD|GCD>x",
      "label": "9"
    },
    {
      "source": "~GCD|GCD>@0#0",
      "target": "~GCD|GCD>x@reset",
      "label": "10"
    },
    {
      "source": "~GCD|GCD>x@init",
      "target": "~GCD|GCD>x",
      "label": "11"
    },
    {
      "source": "~GCD|GCD>y",
      "target": "~GCD|GCD>y@init",
      "label": "12"
    },
    {
      "source": "~GCD|GCD>y",
      "target": "~GCD|GCD>@gt#0",
      "label": "13"
    },
    {
      "source": "~GCD|GCD>y",
      "target": "~GCD|GCD>@sub#0",
      "label": "14"
    },
    {
      "source": "~GCD|GCD>y",
      "target": "~GCD|GCD>@sub#1",
      "label": "15"
    },
    {
      "source": "~GCD|GCD>y",
      "target": "~GCD|GCD>@mux#1",
      "label": "16"
    },
    {
      "source": "~GCD|GCD>y",
      "target": "~GCD|GCD>@eq#1",
      "label": "17"
    },
    {
      "source": "~GCD|GCD>y@clock",
      "target": "~GCD|GCD>y",
      "label": "18"
    },
    {
      "source": "~GCD|GCD>y@reset",
      "target": "~GCD|GCD>y",
      "label": "19"
    },
    {
      "source": "~GCD|GCD>@0#1",
      "target": "~GCD|GCD>y@reset",
      "label": "20"
    },
    {
      "source": "~GCD|GCD>y@init",
      "target": "~GCD|GCD>y",
      "label": "21"
    },
    {
      "source": "~GCD|GCD>_T_14",
      "target": "~GCD|GCD>@mux#0",
      "label": "22"
    },
    {
      "source": "~GCD|GCD>_T_14",
      "target": "~GCD|GCD>@eq#0",
      "label": "23"
    },
    {
      "source": "~GCD|GCD>@gt#0",
      "target": "~GCD|GCD>_T_14",
      "label": "24"
    },
    {
      "source": "~GCD|GCD>_T_15",
      "target": "~GCD|GCD>@tail#0",
      "label": "25"
    },
    {
      "source": "~GCD|GCD>@sub#0",
      "target": "~GCD|GCD>_T_15",
      "label": "26"
    },
    {
      "source": "~GCD|GCD>_T_16",
      "target": "~GCD|GCD>@mux#0",
      "label": "27"
    },
    {
      "source": "~GCD|GCD>@tail#0",
      "target": "~GCD|GCD>_T_16",
      "label": "28"
    },
    {
      "source": "~GCD|GCD>_GEN_0",
      "target": "~GCD|GCD>@mux#2",
      "label": "29"
    },
    {
      "source": "~GCD|GCD>@mux#0",
      "target": "~GCD|GCD>_GEN_0",
      "label": "30"
    },
    {
      "source": "~GCD|GCD>_T_18",
      "target": "~GCD|GCD>@mux#1",
      "label": "31"
    },
    {
      "source": "~GCD|GCD>@eq#0",
      "target": "~GCD|GCD>_T_18",
      "label": "32"
    },
    {
      "source": "~GCD|GCD>@0#2",
      "target": "~GCD|GCD>@eq#0",
      "label": "33"
    },
    {
      "source": "~GCD|GCD>_T_19",
      "target": "~GCD|GCD>@tail#1",
      "label": "34"
    },
    {
      "source": "~GCD|GCD>@sub#1",
      "target": "~GCD|GCD>_T_19",
      "label": "35"
    },
    {
      "source": "~GCD|GCD>_T_20",
      "target": "~GCD|GCD>@mux#1",
      "label": "36"
    },
    {
      "source": "~GCD|GCD>@tail#1",
      "target": "~GCD|GCD>_T_20",
      "label": "37"
    },
    {
      "source": "~GCD|GCD>_GEN_1",
      "target": "~GCD|GCD>@mux#3",
      "label": "38"
    },
    {
      "source": "~GCD|GCD>@mux#1",
      "target": "~GCD|GCD>_GEN_1",
      "label": "39"
    },
    {
      "source": "~GCD|GCD>_GEN_2",
      "target": "~GCD|GCD>x",
      "label": "40"
    },
    {
      "source": "~GCD|GCD>@mux#2",
      "target": "~GCD|GCD>_GEN_2",
      "label": "41"
    },
    {
      "source": "~GCD|GCD>io.e",
      "target": "~GCD|GCD>@mux#2",
      "label": "42"
    },
    {
      "source": "~GCD|GCD>io.e",
      "target": "~GCD|GCD>@mux#3",
      "label": "43"
    },
    {
      "source": "~GCD|GCD>io.a",
      "target": "~GCD|GCD>@mux#2",
      "label": "44"
    },
    {
      "source": "~GCD|GCD>_GEN_3",
      "target": "~GCD|GCD>y",
      "label": "45"
    },
    {
      "source": "~GCD|GCD>@mux#3",
      "target": "~GCD|GCD>_GEN_3",
      "label": "46"
    },
    {
      "source": "~GCD|GCD>io.b",
      "target": "~GCD|GCD>@mux#3",
      "label": "47"
    },
    {
      "source": "~GCD|GCD>_T_22",
      "target": "~GCD|GCD>io.v",
      "label": "48"
    },
    {
      "source": "~GCD|GCD>@eq#1",
      "target": "~GCD|GCD>_T_22",
      "label": "49"
    },
    {
      "source": "~GCD|GCD>@0#3",
      "target": "~GCD|GCD>@eq#1",
      "label": "50"
    }
  ]
}