#pragma once
#include"../include/SInt.h"
#include"../include/pch.h"

SInt::SInt(bool sign, int w, long long x)
	:sign(sign)
{
	this->pnum = new UInt(w, x);
}

SInt::SInt(const SInt &x) {
	this->pnum = new UInt(*x.pnum);
	this->sign = x.sign;
	*this = x;
}

SInt::~SInt() {
	if (!pnum) {
		delete pnum;
	}
}

SInt& SInt::operator=(long long x) {
	sign = x > 0 ? true : false;
	this->pnum->num.clear();
	(*this->pnum) = x;
	return *this;
}

SInt& SInt::operator=(const std::string &str) {
	(*this->pnum) = str;
	return *this;
}

SInt& SInt::operator=(const SInt &tmp) {
	(*this->pnum) = *tmp.pnum;
	return *this;
}

SInt SInt::abs() const {
	SInt ans(*this);
	ans.sign = false;
	return ans;
}

SInt SInt::operator+() const { return *this; }

SInt SInt::operator-() const
{
	SInt ans(*this);
	if (ans != 0)
		ans.sign = !ans.sign;
	return ans;
}

bool SInt::getflag() {
	return this->pnum->num[pnum->num.size() - 1] >> (pnum->w%UInt::WIDTH) == 0;
}

SInt SInt::operator+(const SInt &b) const {
	SInt ans(false, std::max(this->pnum->w, b.pnum->w)+1, 0);
	*ans.pnum = *this->pnum + *b.pnum;
	ans.sign = ans.getflag();
	return ans;
}

SInt SInt::operator-(const SInt &b) const {
	return *this + (-b);
}

SInt SInt::operator*(const SInt &b) const {
	SInt ans(false, this->pnum->w + b.pnum->w, 0);
	*ans.pnum = (*this->pnum) * (*b.pnum);
	ans.sign = !(sign == b.sign);
	return ans;
}

SInt SInt::operator/(const SInt &b) const {
	SInt bb(b.abs());
	SInt aa(this->abs());
	SInt ans(b.sign&&sign,0);
	*(ans.pnum) = (*aa.pnum) / (*b.pnum);
	ans.absTocom();
	return ans;
}

bool SInt::opearator() const {
	return this->pnum->opearator();
}

SInt SInt::operator%(const SInt &b) const
{
	SInt bb(b.abs());
	SInt aa(this->abs());
	SInt ans(b.sign&&sign, 0);
	*(ans.pnum) = (*aa.pnum) % (*b.pnum);
	ans.absTocom();
	return ans;
}

SInt& SInt::operator++()
{
	*this = *this + 1;
	return *this;
}
SInt& SInt::operator--()
{
	*this = *this - 1;
	return *this;
}
SInt& SInt::operator+=(const SInt &b)
{
	*this = *this + b;
	return *this;
}
SInt& SInt::operator-=(const SInt &b)
{
	*this = *this - b;
	return *this;
}

SInt& SInt::operator*=(const SInt &b)
{
	*this = *this * b;
	return *this;
}

SInt& SInt::operator/=(const SInt &b)
{
	*this = *this / b;
	return *this;
}

SInt& SInt::operator%=(const SInt &b)
{
	*this = *this % b;
	return *this;
}

bool SInt::operator<(const SInt &b) const
{
	if (sign != b.sign) {
		return !sign;
	}
	else if (!sign && !b.sign) {
		return -b < -*this;
	}
	return *this->pnum < *b.pnum;
}

SInt SInt::operator<<(const int a) const {
	SInt res(this->sign, this->pnum->w - a, 0);
	*res.pnum = *this->pnum >> a;
	return res;
}

inline SInt SInt::operator~() const
{
	SInt res(*this);
	*res.pnum = ~ *res.pnum;
	return res;
}

SInt SInt::operator>>(const int a) const {
	SInt res(this->sign, this->pnum->w - a, 0);
	*res.pnum = *this->pnum >> a;
	return res;
}

SInt SInt::operator&(const SInt&b) const {
	SInt res(this->sign, this->pnum->w, 0);
	*res.pnum = *this->pnum & * b.pnum;
	return res;
}

SInt SInt::operator|(const SInt&b) const {
	SInt res(this->sign, this->pnum->w, 0);
	*res.pnum = *this->pnum | * b.pnum;
	return res;
}

SInt SInt::operator^(const SInt&b) const {
	SInt res(this->sign, this->pnum->w, 0);
	*res.pnum = *this->pnum ^ * b.pnum;
	return res;
}

SInt SInt::bits(int low, int high) const {
	SInt res(this->sign, high - low + 1, 0);
	res = *this >> low;
	return res;
}

SInt SInt::cat(const SInt & b) const {
	SInt res(*this | b << this->pnum->w);
	return res;
}

SInt SInt::tail(int n) const {
	SInt res(this->sign, this->pnum->w - n, 0);
	res = *this;
	return res;
}

SInt SInt::pad(int n) const {
	SInt res(this->sign, this->pnum->w - n, 0);
	res = *this;
	return res;
}

bool SInt::operator> (const SInt &b) const { return b < *this; }
bool SInt::operator<=(const SInt &b) const { return !(b < *this); }
bool SInt::operator>=(const SInt &b) const { return !(*this < b); }
bool SInt::operator!=(const SInt &b) const { return b < *this || *this < b; }
bool SInt::operator==(const SInt &b) const { return !(b < *this) && !(*this < b); }																		// �߼������
bool SInt::operator||(const SInt &b) const { return *this != 0 || b != 0; }
bool SInt::operator&&(const SInt &b) const { return *this != 0 && b != 0; }
bool SInt::operator!() { return (bool)(*this == 0); }

void SInt::absTocom() {
	static bool flag = false;
	if (this->sign) {
		return;
	}
	*this->pnum = ~(*this->pnum) + 1;
}

std::ostream &operator<<(std::ostream &out, const SInt &x)
{
	if (!x.sign)
		out << '-';
	out << x.pnum->num.back();
	for (int i = x.pnum->num.size() - 2; i >= 0; i--) {
		char buf[10];
		//��WIDTH��BASR�б仯,�˴�Ҫ�޸�Ϊ%0(WIDTH)d
		sprintf(buf, "%08x", x.pnum->num[i]);
		for (int j = 0; j < strlen(buf); j++)
			out << buf[j];
	}
	return out;
}
std::istream &operator>>(std::istream &in, SInt &x)
{
	std::string str;
	in >> str;
	size_t len = str.size();
	int start = 0;
	if (str[0] == '-')
		start = 1;
	if (str[start] == '\0')
		return in;
	for (int i = start; i < len; i++) {
		if (str[i] < '0' || str[i] > '9')
			return in;
	}
	x.sign = !start;
	//x = str.c_str();
	return in;
}