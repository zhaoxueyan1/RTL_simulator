#pragma once
#include"pch.h"
#include"Type.h"

template<int w>
class UInt:public Type<w> 
{
public:
	
	UInt();
	UInt(UINT_64 w);
	~UInt();

	operator UINT_64()const;
	//operator bool()const;
	UInt<w>& operator=(const UInt<w>& b){
		this->data = b.data;
		return *this;
	}

	void setData(UINT_64 b) {
		*this = b;
	}
	
	friend inline const UInt<w> operator+(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a) + UINT_64(b); }
	friend inline const UInt<w> operator-(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a) - UINT_64(b); }
	friend inline const UInt<w> operator*(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a) * UINT_64(b); }
	friend inline const UInt<w> operator/(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a) / UINT_64(b); }
	friend inline const UInt<w> operator|(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a) | UINT_64(b); }
	friend inline const UInt<w> operator&(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a) & UINT_64(b); }
	friend inline const UInt<w> operator^(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a) ^ UINT_64(b); }
	friend inline const UInt<w> operator>>(const UInt<w>& a, int shift) { return UINT_64(a) >>= shift; }
	friend inline const UInt<w> operator<<(const UInt<w>& a, int shift) { return UINT_64(a) <<= shift; }
	friend inline const UInt<w> operator*(const UInt<w>& a, UINT_64 b) { return UINT_64(a) *= b; }
	friend inline bool operator!=(const UInt<w>& a, const UInt<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) != 0; }
	friend inline bool operator!=(const UInt<w>& a, UINT_64 b) { return !(a - b == 0); }
	friend inline bool operator>(const UInt<w>& a, const UInt<w>& b)  { return UINT_64(a)>UINT_64(b); }
	friend inline bool operator<(const UInt<w>& a, const UInt<w>& b)  { return UINT_64(a)<UINT_64(b); }
	friend inline bool operator>=(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a)>=UINT_64(b); }
	friend inline bool operator<=(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a)<=UINT_64(b); }
	//template<int u> friend inline bool operator==(const UInt<w>& a, const UInt<u>& b) { return UINT_64(a) == UINT_64(b); }
	//friend inline bool operator==(const UInt<w>& a, UINT_64 b) { return a-b == 0; }
	//friend inline bool operator==(const UInt<w>& a, bool b) { return a-b == 0; }
	
	bool operator==(const UINT_64 b)const {
		return UINT_64(*this) - b == 0;
	}
	template<int up,int lo>
	UInt<up - lo + 1> & bits() {
		UINT_64 t = UINT_64(*this)>>lo;
		UINT_64 res = 0;
		for (int i = 0; i < up - lo + 1; i++) {
			res |= t & 1 << i;
		}
		return *(new UInt<up-lo+1>(res));
	}
	//UInt
	
	template<int u> 
	const UInt<w-u>& tail()const {
		return *(new UInt<w-u>(*this));
	}
	
	template<int u>
	UInt(const UInt<u>& b) {
		this->data = b.data;
	}
};

template<int w>
inline UInt<w>::UInt()
{
	this->data = 0;
}

template<int w>
inline UInt<w>::UInt(UINT_64 w)
{
	this->data = w;
}

template<int w>
inline UInt<w>::~UInt()
{

}



template<int w>
UInt<w>::operator UINT_64() const
{
	return this->data;
}

//template<int w>
//inline UInt<w>::operator bool() const
//{
//	return UINT_64()
//}

//template<int w>
//UInt<w>::operator bool()
//{
//	return *this == 0 ? 0 : 1;
//}