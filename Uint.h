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
	UInt<w>& operator=(const UInt<w>& b) {
		memcpy(this->data, b.data, this->t);
		return *this;
	}

	void setData(UINT_64 b) {
		*this = b;
	}
	
	friend inline const UInt<w> operator+(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UINT_64(a) + UINT_64(b)); }
	friend inline const UInt<w> operator-(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UINT_64(a) - UINT_64(b)); }
	friend inline const UInt<w> operator*(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UINT_64(a) * UINT_64(b)); }
	friend inline const UInt<w> operator/(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UINT_64(a) / UINT_64(b)); }
	friend inline const UInt<w> operator|(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UINT_64(a) | UINT_64(b)); }
	friend inline const UInt<w> operator&(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UINT_64(a) & UINT_64(b)); }
	friend inline const UInt<w> operator^(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UINT_64(a) ^ UINT_64(b)); }
	friend inline const UInt<w> operator>>(const UInt<w>& a, int shift) { return UINT_64(a) >>= shift; }
	friend inline const UInt<w> operator<<(const UInt<w>& a, int shift) { return UINT_64(a) <<= shift; }
	friend inline const UInt<w> operator*(const UInt<w>& a, UINT_64 b) { return UINT_64(a) *= b; }
	friend inline bool operator!=(const UInt<w>& a, const UInt<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) != 0; }
	friend inline bool operator!=(const UInt<w>& a, UINT_64 b) { return !(a - b == 0); }
	friend inline bool operator>(const UInt<w>& a, const UInt<w>& b)  { return UINT_64(a)-UINT_64(b) > 0; }
	friend inline bool operator<(const UInt<w>& a, const UInt<w>& b)  { return UINT_64(a)-UINT_64(b) < 0; }
	friend inline bool operator>=(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a)-UINT_64(b) >= 0; }
	friend inline bool operator<=(const UInt<w>& a, const UInt<w>& b) { return UINT_64(a)-UINT_64(b) <= 0; }
	//template<int u> friend inline bool operator==(const UInt<w>& a, const UInt<u>& b) { return UINT_64(a) == UINT_64(b); }
	//friend inline bool operator==(const UInt<w>& a, UINT_64 b) { return a-b == 0; }
	//friend inline bool operator==(const UInt<w>& a, bool b) { return a-b == 0; }
	
	bool operator==(const UINT_64 b)const {
		return UINT_64(*this) - b == 0;
	}
	template<int lo,int up>
	UInt<up - lo + 1> & bits() {
		UINT_64 t = UINT_64(*this)>>lo;
		UINT_64 res = 0;
		for (int i = 0; i < up - lo + 1; i++) {
			res |= t & 1 << i;
		}
		return UInt<up-lo+1>(res);
	}
	//UInt
	
	template<int u> 
	const UInt<w-u>& tail()const {
		return UInt<w-u>(*this);
	}
	
	template<int u>
	UInt(const UInt<u>& b) {
		//this->t = cal();
		memcpy(this->data,b.data,min(this->t,b.t));
	}
};

template<int w>
inline UInt<w>::UInt()
{

}

template<int w>
inline UInt<w>::UInt(UINT_64 w)
{
	memcpy(this->data, &w, this->t);
}

template<int w>
inline UInt<w>::~UInt()
{

}



template<int w>
UInt<w>::operator UINT_64() const
{
	switch (this->t)
	{
	case 1: return *((UINT_8*)this->data); 
	case 2: return *((UINT_16*)this->data); 
	case 4: return *((UINT_32*)this->data); 
	case 8: return *((UINT_64*)this->data);
	default:
		break;
	}
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