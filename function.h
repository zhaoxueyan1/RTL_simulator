#include"pch.h"
#include"UInt.h"
#include"Sint.h"
namespace FuncType
{
	enum
	{
		Wire,
		Node,
		Port,
		Reg,
		Mux,
		UIntLiteral,
		Eq,
		Bits, // expr
		And,
		Or,
		Cat,
		Add,
		ValidIf,
		Tail, // expr
		AsUInt, // expr
		Not,
		Memen, // expr
		Memdata,
		Memclk,
		Memaddr,
		Memmask,
		Neq,
		Pad,//
		Lt,
		Geq,
		Xor,
		Dshr,//
		Dshl,//
		Sub,
		Gt,
		Shl,//
		Shr,//
		AsSInt,
		Mul,
		Leq,
	};
};

template<typename T> inline T Mux(T &a, T &b, T &c) { bool(a) ? b : c; }
template<typename T> inline T Eq(T &a, T &b) { return a == b; }
template<typename T> inline T Bits    (T &a, int low, int high) { return a.bits(low,high); }
template<typename T> inline T And(T &a, T &b) { return a & b; } 
template<typename T> inline T Or(T &a, T &b) { return a | b; }
template<typename T> inline T Cat(T &a, T &b) { return a.cat(b); }
template<typename T> inline T ValidIf(T &a, T &b) { return bool(a) ? b : rand(); }
template<typename T> inline T Add(T &a, T &b) { return a + b; }
template<typename T> inline T Tail(T &a, int c) { return a.tail(c);}
template<typename T> inline T Not(T &a) { return !(bool )a; }
template<typename T> inline T Neq(T &a, T &b) { return a != b; }
template<typename T> inline T Lt(T &a, T &b) { return a < b; }
template<typename T> inline T Geq(T &a, T &b) { return a >= b; }
template<typename T> inline T Xor(T &a, T &b) { return a ^ b; }
template<typename T> inline T Sub(T &a, T &b) { return a - b; }
template<typename T> inline T Gt(T &a, T &b) { return a > b; }
template<typename T> inline T Mul(T &a, T &b) { return a * b; }
template<typename T> inline T Leq(T &a, T &b) { return a <= b; }
template<typename T> inline T Pad(T &a, int n) { return a.pad(n);}
template<typename T> inline T Dshr(T &a, int b) {}
template<typename T> inline T Dshl(T &a, int b) {}
template<typename T> inline T Shl(T &a, int b) {}
template<typename T> inline T Shr(T &a, int b) {}
template<typename T> inline T Memen(T &a, T &b) {}
template<typename T> inline T Memdata(T &a, T &b) {}
template<typename T> inline T Memclk(T &a, T &b) {}
template<typename T> inline T Memaddr(T &a, T &b) {}
template<typename T> inline T Memmask(T &a, T &b) {}

inline UInt AsUInt(SInt & b)
{
	if (b.sign) {
		return *b.pnum;
	}
	else {
		SInt a(b);
		a.absTocom();
		return *a.pnum;
	}
}

inline SInt AsSInt(UInt & b){
	SInt ans(true,b.w,0);
	*ans.pnum = b;
	return ans;
}
