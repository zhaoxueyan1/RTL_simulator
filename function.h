#include"pch.h"
#include"Number.h"
namespace FuncType
{
	enum
	{
		Wire,
		Node,
		Mux,
		UIntLiteral,
		Eq,
		Port,
		Bits,
		And,
		Reg,
		Or,
		Cat,
		ValidIf,
		Add,
		Tail,
		AsUInt,
		Not,
		Memen,
		Memdata,
		Memclk,
		Memaddr,
		Neq,
		Pad,
		Memmask,
		Lt,
		Geq,
		Xor,
		Dshr,
		Dshl,
		Sub,
		Gt,
		Shl,
		Shr,
		AsSInt,
		Mul,
	};
};


inline Number Mux     (Number &a, Number &b,Number &c) {}
inline Number Eq	  (Number &a,Number &b){}
inline Number Bits	  (Number &a,Number &b){}
inline Number And	  (Number &a,Number &b){}
inline Number Reg	  (Number &a,Number &b){} 
inline Number Or	  (Number &a,Number &b){}
inline Number Cat	  (Number &a,Number &b){}
inline Number ValidIf (Number &a,Number &b){}
inline Number Add	  (Number &a,Number &b){}
inline Number Tail	  (Number &a,Number &b){}
inline Number AsUInt  (Number &a,Number &b){}
inline Number Not	  (Number &a,Number &b){}
inline Number Memen   (Number &a,Number &b){}
inline Number Memdata (Number &a,Number &b){}
inline Number Memclk  (Number &a,Number &b){}
inline Number Memaddr (Number &a,Number &b){}
inline Number Neq	  (Number &a,Number &b){}
inline Number Pad	  (Number &a,Number &b){}
inline Number Memmask (Number &a,Number &b){} 
inline Number Lt	  (Number &a,Number &b){}
inline Number Geq	  (Number &a,Number &b){}
inline Number Xor	  (Number &a,Number &b){}
inline Number Dshr	  (Number &a,Number &b){}
inline Number Dshl	  (Number &a,Number &b){}
inline Number Sub	  (Number &a,Number &b){}
inline Number Gt	  (Number &a,Number &b){}
inline Number Shl	  (Number &a,Number &b){}
inline Number Shr	  (Number &a,Number &b){}
inline Number AsSInt  (Number &a,Number &b){}
inline Number Mul	  (Number &a,Number &b){}