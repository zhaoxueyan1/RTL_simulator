#pragma once
#include"pch.h"
#include"Type.h"

template<int w>
class Uint:public Type<w> 
{
public:
	Uint();
	Uint(UINT_64 w);
	~Uint();

	operator UINT_64()const;
	Uint<w>& operator=(const Uint<w>& b) {
		return Uint<w>(b);
	}
	//operator bool();
	friend inline const Uint<w> operator+(const Uint<w>& a, const Uint<w>& b) { return Uint<w>(UINT_64(a) + UINT_64(b)); }
	friend inline const Uint<w> operator-(const Uint<w>& a, const Uint<w>& b) { return Uint<w>(UINT_64(a) - UINT_64(b)); }
	friend inline const Uint<w> operator*(const Uint<w>& a, const Uint<w>& b) { return Uint<w>(UINT_64(a) * UINT_64(b)); }
	friend inline const Uint<w> operator/(const Uint<w>& a, const Uint<w>& b) { return Uint<w>(UINT_64(a) / UINT_64(b)); }
	friend inline const Uint<w> operator|(const Uint<w>& a, const Uint<w>& b) { return Uint<w>(UINT_64(a) | UINT_64(b)); }
	friend inline const Uint<w> operator&(const Uint<w>& a, const Uint<w>& b) { return Uint<w>(UINT_64(a) & UINT_64(b)); }
	friend inline const Uint<w> operator^(const Uint<w>& a, const Uint<w>& b) { return Uint<w>(UINT_64(a) ^ UINT_64(b)); }
	friend inline const Uint<w> operator>>(const Uint<w>& a, int shift) { return UINT_64(a) >>= shift; }
	friend inline const Uint<w> operator<<(const Uint<w>& a, int shift) { return UINT_64(a) <<= shift; }
	friend inline const Uint<w> operator*(const Uint<w>& a, uint32_t b) { return UINT_64(a) *= b; }
	friend inline bool operator==(const Uint<w>& a, const Uint<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) == 0; }
	friend inline bool operator!=(const Uint<w>& a, const Uint<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) != 0; }
	friend inline bool operator>(const Uint<w>& a, const Uint<w>& b)  { return a-b > 0; }
	friend inline bool operator<(const Uint<w>& a, const Uint<w>& b)  { return a-b < 0; }
	friend inline bool operator>=(const Uint<w>& a, const Uint<w>& b) { return a-b >= 0; }
	friend inline bool operator<=(const Uint<w>& a, const Uint<w>& b) { return a-b <= 0; }
	friend inline bool operator==(const Uint<w>& a, uint64_t b) { return a-b == 0; }
	friend inline bool operator!=(const Uint<w>& a, uint64_t b) { return !(a-b == 0); }

	template<int u> 
	const Uint<w-u>& tail()const {
		return Uint<w-u>(*this);
	}
	
	template<int u>
	Uint(const Uint<u>& b) {
		//this->t = cal();
		memcpy(this->data,b.data,min(this->t,b.t));
	}
};

template<int w>
inline Uint<w>::Uint()
{

}

template<int w>
inline Uint<w>::Uint(UINT_64 w)
{
	memcpy(this->data, &w, this->t);
}

template<int w>
inline Uint<w>::~Uint()
{

}



template<int w>
Uint<w>::operator UINT_64() const
{
	switch (this->t)
	{
	case 1: return *((UINT_8 *)this->data); 
	case 2: return *((UINT_16*)this->data); 
	case 4: return *((UINT_32*)this->data); 
	case 8: return *((UINT_64*)this->data);
	default:
		break;
	}
}

//template<int w>
//Uint<w>::operator bool()
//{
//	return *this == 0 ? 0 : 1;
//}