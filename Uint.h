#pragma once
#include"pch.h"
#include"Type.h"

template<int w>
class UInt:public Type<w> 
{
public:
	UInt();
	UInt(UInt_64 w);
	~UInt();

	operator UInt_64()const;
	UInt<w>& operator=(const UInt<w>& b) {
		return UInt<w>(b);
	}
	//operator bool();
	friend inline const UInt<w> operator+(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UInt_64(a) + UInt_64(b)); }
	friend inline const UInt<w> operator-(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UInt_64(a) - UInt_64(b)); }
	friend inline const UInt<w> operator*(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UInt_64(a) * UInt_64(b)); }
	friend inline const UInt<w> operator/(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UInt_64(a) / UInt_64(b)); }
	friend inline const UInt<w> operator|(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UInt_64(a) | UInt_64(b)); }
	friend inline const UInt<w> operator&(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UInt_64(a) & UInt_64(b)); }
	friend inline const UInt<w> operator^(const UInt<w>& a, const UInt<w>& b) { return UInt<w>(UInt_64(a) ^ UInt_64(b)); }
	friend inline const UInt<w> operator>>(const UInt<w>& a, int shift) { return UInt_64(a) >>= shift; }
	friend inline const UInt<w> operator<<(const UInt<w>& a, int shift) { return UInt_64(a) <<= shift; }
	friend inline const UInt<w> operator*(const UInt<w>& a, UInt32_t b) { return UInt_64(a) *= b; }
	friend inline bool operator==(const UInt<w>& a, const UInt<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) == 0; }
	friend inline bool operator!=(const UInt<w>& a, const UInt<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) != 0; }
	friend inline bool operator>(const UInt<w>& a, const UInt<w>& b)  { return a-b > 0; }
	friend inline bool operator<(const UInt<w>& a, const UInt<w>& b)  { return a-b < 0; }
	friend inline bool operator>=(const UInt<w>& a, const UInt<w>& b) { return a-b >= 0; }
	friend inline bool operator<=(const UInt<w>& a, const UInt<w>& b) { return a-b <= 0; }
	friend inline bool operator==(const UInt<w>& a, UInt64_t b) { return a-b == 0; }
	friend inline bool operator!=(const UInt<w>& a, UInt64_t b) { return !(a-b == 0); }

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
inline UInt<w>::UInt(UInt_64 w)
{
	memcpy(this->data, &w, this->t);
}

template<int w>
inline UInt<w>::~UInt()
{

}



template<int w>
UInt<w>::operator UInt_64() const
{
	switch (this->t)
	{
	case 1: return *((UInt_8 *)this->data); 
	case 2: return *((UInt_16*)this->data); 
	case 4: return *((UInt_32*)this->data); 
	case 8: return *((UInt_64*)this->data);
	default:
		break;
	}
}

//template<int w>
//UInt<w>::operator bool()
//{
//	return *this == 0 ? 0 : 1;
//}