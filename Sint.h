#pragma once
#include"pch.h"
#include"Type.h"

template<int w>
class Sint :public Type<w>
{
public:
	Sint();
	Sint(SINT_64 w);
	~Sint();

	operator SINT_64()const;
	Sint<w>& operator=(const Sint<w>& b) {
		return Sint<w>(b);
	}
	//operator bool();
	friend inline const Sint<w> operator+(const Sint<w>& a, const Sint<w>& b) { return Sint<w>(Sint_64(a) + Sint_64(b)); }
	friend inline const Sint<w> operator-(const Sint<w>& a, const Sint<w>& b) { return Sint<w>(Sint_64(a) - Sint_64(b)); }
	friend inline const Sint<w> operator*(const Sint<w>& a, const Sint<w>& b) { return Sint<w>(Sint_64(a) * Sint_64(b)); }
	friend inline const Sint<w> operator/(const Sint<w>& a, const Sint<w>& b) { return Sint<w>(Sint_64(a) / Sint_64(b)); }
	friend inline const Sint<w> operator|(const Sint<w>& a, const Sint<w>& b) { return Sint<w>(Sint_64(a) | Sint_64(b)); }
	friend inline const Sint<w> operator&(const Sint<w>& a, const Sint<w>& b) { return Sint<w>(Sint_64(a) & Sint_64(b)); }
	friend inline const Sint<w> operator^(const Sint<w>& a, const Sint<w>& b) { return Sint<w>(Sint_64(a) ^ Sint_64(b)); }
	friend inline const Sint<w> operator>>(const Sint<w>& a, int shift) { return Sint_64(a) >>= shift; }
	friend inline const Sint<w> operator<<(const Sint<w>& a, int shift) { return Sint_64(a) <<= shift; }
	friend inline const Sint<w> operator*(const Sint<w>& a, Sint32_t b) { return Sint_64(a) *= b; }
	friend inline bool operator==(const Sint<w>& a, const Sint<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) == 0; }
	friend inline bool operator!=(const Sint<w>& a, const Sint<w>& b) { return memcmp(a.data, b.data, sizeof(a.t)) != 0; }
	friend inline bool operator>(const Sint<w>& a, const Sint<w>& b) { return a - b > 0; }
	friend inline bool operator<(const Sint<w>& a, const Sint<w>& b) { return a - b < 0; }
	friend inline bool operator>=(const Sint<w>& a, const Sint<w>& b) { return a - b >= 0; }
	friend inline bool operator<=(const Sint<w>& a, const Sint<w>& b) { return a - b <= 0; }
	friend inline bool operator==(const Sint<w>& a, Sint64_t b) { return a - b == 0; }
	friend inline bool operator!=(const Sint<w>& a, Sint64_t b) { return !(a - b == 0); }

	template<int u>
	const Sint<w - u>& tail()const {
		return Sint<w - u>(*this);
	}

	template<int u>
	Sint(const Sint<u>& b) {
		//this->t = cal();
		memcpy(this->data, b.data, min(this->t, b.t));
	}
};

template<int w>
inline Sint<w>::Sint()
{

}

template<int w>
inline Sint<w>::Sint(SINT_64 w)
{
	memcpy(this->data, &w, this->t);
}

template<int w>
inline Sint<w>::~Sint()
{

}



template<int w>
Sint<w>::operator SINT_64() const
{
	switch (this->t)
	{
	case 1: return *((SINT_8 *)this->data);
	case 2: return *((SINT_16*)this->data);
	case 4: return *((SINT_32*)this->data);
	case 8: return *((SINT_64*)this->data);
	default:
		break;
	}
}

//template<int w>
//Sint<w>::operator bool()
//{
//	return *this == 0 ? 0 : 1;
//}