#pragma once
#include"pch.h"
#include<vector>
#include<string>

struct Number
{
	int w;
	// True  :postive , False :negtive
	bool sign;

	std::vector<uint32_t> num;
	uint32_t mask;
	static const int WIDTH = 32;
	static const uint32_t BASE = 0Xffffffff;
	
	Number(bool sign = true, int w = 32, long long x = 0);
	Number(const Number &x);
	
	Number& operator=(long long x);
	Number& operator=(const std::string &str);
	
	Number abs() const;
	Number operator+() const;
	Number operator-() const;
	Number operator+(const Number &b) const;
	Number operator-(const Number &b) const;
	Number operator*(const Number &b) const;
	Number operator/(const Number &b) const;
	Number operator%(const Number &b) const;
	
	bool opearator() const;
	
	Number& operator++();
	Number& operator--();
	Number& operator+=(const Number &b);
	Number& operator-=(const Number &b);
	Number& operator*=(const Number &b);
	Number& operator/=(const Number &b);
	Number& operator%=(const Number &b);
	
	Number operator<<(const int a) const;
	Number operator>>(const int a) const;
	Number operator& (const Number&b) const;
	Number operator| (const Number&b) const;
	Number operator^ (const Number&b) const;
	Number bits(int low, int high) const;
	Number cat (const Number & b) const;
	Number tail(int n) const;
	Number pad (int n) const;
	
	bool operator< (const Number &b) const;
	bool operator> (const Number &b) const;
	bool operator<=(const Number &b) const;
	bool operator>=(const Number &b) const;
	bool operator!=(const Number &b) const;
	bool operator==(const Number &b) const;
	bool operator||(const Number &b) const;
	bool operator&&(const Number &b) const;
	bool operator! ();
	friend std::ostream &operator<<(std::ostream &out, const Number &x);
	friend std::istream &operator>>(std::istream &in, Number &x);
};

Number::Number(bool sign , int w, long long x)
	:sign(sign), w(w)
{
	*this = x;
	int t = w % (WIDTH + 1);
	mask = t == WIDTH ? BASE : (1 << t + 1) - 1;
}

Number::Number(const Number &x) {
	this->w = x.w;
	for (int i = 0; i < ceil(1.*w / WIDTH) - num.size(); i++) {
		num.push_back(0);
	}
	*this = x;
}

Number& Number::operator=(long long x) {
	sign = x > 0 ? true : false;
	num.clear();
	do {
		num.push_back(x & BASE);
		x >> (WIDTH);
	} while (x > 0);
	for (int i = 0; i < ceil(1.*w / WIDTH) - num.size(); i++) {
		num.push_back(0);
	}
	return *this;
}

Number& Number::operator=(const std::string &str) {
	num.clear();
	sign = (str[0] != '-'); //���÷���
	int ww = WIDTH / 4;
	int x, len = ceil(1.* (str.size() - 1 - (!sign)) / ww);
	for (int i = 0; i < len; i++) {
		int End = str.size() - i * ww;
		int start = std::max((int)(!sign), End - ww); //��ֹԽ��
		sscanf(str.substr(start, End - start).c_str(), "%x", &x);
		num[i] = x;
	}
	for (int i = 0; i < ceil(1.*w / WIDTH) - num.size(); i++) {
		num.push_back(0);
	}
	return *this;
}
Number& Number::operator=(const Number &tmp) {
	if (this == &tmp) return *this;
	sign = tmp.sign;
	for (int i = 0; i < num.size(); i++) {
		num[i] = tmp.num[i];
	}
	num[num.size() - 1] = num[num.size() - 1] & mask;
	return *this;
}

Number Number::abs() const {
	Number ans(*this);
	ans.sign = false;
	return ans;
}

Number Number::operator+() const { return *this; }

Number Number::operator-() const
{
	Number ans(*this);
	if (ans != 0)
		ans.sign = !ans.sign;
	return ans;
}

Number Number::operator+(const Number &b) const {
	if (!b.sign) {
		return *this - (-b);
	}
	if (!sign) {
		return b - (-*this);
	}
	Number ans(false, std::max(this->w, b.w), 0);
	for (int i = 0, g = 0;; i++) {
		if (g == 0 && i >= num.size() && i >= b.num.size())
			break;
		int x = g;
		if (i < num.size())
			x += num[i];
		if (i < b.num.size())
			x += b.num[i];
		ans.num[i] = x & BASE;
		g = x >> WIDTH;
	}
	return ans;
}

Number Number::operator-(const Number &b) const {
	Number ans(false, std::max(this->w, b.w) + 1, 0);
	if (b.sign) {
		return *this + (-b);
	}
	if (sign) {
		return -((-*this) + b);
	}
	long long g = 0;
	if ((*this) < b) {
		/*return -(b - *this);*/
		ans.sign = false;
		long long g = 0;
		for (int i = 0;; i++) {
			if (g == 0 && i >= num.size() && i >= b.num.size())
				break;
			long long x = g;
			g = 0;
			if (i < b.num.size())
				x += b.num[i];
			if (i < num.size())
				x -= num[i];
			if (x < 0) {
				x += BASE;
				g = -1;
			}
			ans.num[i] = x;
		}
	}
	else {
		for (int i = 0;; i++) {
			if (g == 0 && i >= num.size() && i >= b.num.size())
				break;
			long long x = g;
			g = 0;
			if (i < num.size())
				x += num[i];
			if (i < b.num.size())
				x -= b.num[i];
			if (x < 0) {
				x += BASE;
				g = -1;
			}
			ans.num[i] = x;
		}
	}
	return ans;
}

Number Number::operator*(const Number &b) const {
	int lena = num.size(), lenb = b.num.size();
	Number ans(false, this->w + b.w, 0);
	for (int i = 0, g = 0; i < lena; i++) {
		g = 0;
		for (int j = 0; j < lenb; j++) {
			long long x = ans.num[i + j];
			x += (long long)num[i] * (long long)b.num[j];
			ans.num[i + j] = x & BASE;
			g = x >> WIDTH;
			ans.num[i + j + 1] += g;
		}
	}
	ans.sign = !(sign == b.sign);
	return ans;
}

Number Number::operator/(const Number &b) const {
	Number bb(b.abs());
	Number aa(this->abs());
	if (*this < bb)
		return 0;
	Number tmp(this->sign, bb.w + 1, 0);
	Number ans(this->sign, w, 0);
	for (int i = tmp.num.size() - 2; i >= 0; i++) {
		tmp.num[i] = num[i];
	}
	int lena = this->num.size(), lenb = bb.num.size();
	for (int i = 0; i <= lena - lenb; i++) {
		long long t = 0;
		while (tmp >= bb) {
			t++;
			tmp = tmp - bb;
		}
		for (int i = bb.num.size(); i > 0; i--) {
			tmp.num[i] = tmp.num[i - 1];
		}
		ans.num[lena - lenb - i] = t;
		if (i == lena - lenb) break;
		tmp.num[0] = this->num[lenb + i];
	}
	ans.sign = (ans == 0 || sign == b.sign);
	return ans;
}

bool Number::opearator() const {
	return 1;
}

Number Number::operator%(const Number &b) const
{
	Number bb(b.abs());
	Number aa(this->abs());
	if (*this < bb)
		return 0;
	Number tmp(this->sign, bb.w + 1, 0);
	Number ans(this->sign, w, 0);
	for (int i = tmp.num.size() - 2; i >= 0; i++) {
		tmp.num[i] = num[i];
	}
	int lena = this->num.size(), lenb = bb.num.size();
	for (int i = 0; i <= lena - lenb; i++) {
		long long t = 0;
		while (tmp >= bb) {
			t++;
			tmp = tmp - bb;
		}
		if (i == lena - lenb) break;
		for (int i = bb.num.size(); i > 0; i--) {
			tmp.num[i] = tmp.num[i - 1];
		}
		ans.num[lena - lenb - i] = t;
		tmp.num[0] = this->num[lenb + i];
	}
	tmp.sign = (ans == 0 || sign == b.sign);
	return tmp;
}

Number& Number::operator++()
{
	*this = *this + 1;
	return *this;
}
Number& Number::operator--()
{
	*this = *this - 1;
	return *this;
}
Number& Number::operator+=(const Number &b)
{
	*this = *this + b;
	return *this;
}
Number& Number::operator-=(const Number &b)
{
	*this = *this - b;
	return *this;
}

Number& Number::operator*=(const Number &b)
{
	*this = *this * b;
	return *this;
}

Number& Number::operator/=(const Number &b)
{
	*this = *this / b;
	return *this;
}

Number& Number::operator%=(const Number &b)
{
	*this = *this % b;
	return *this;
}

bool   Number::operator<(const Number &b) const
{
	if (sign != b.sign) //����������
	{
		return !sign;
	}
	else if (!sign && !b.sign) //����
	{
		return -b < -*this;
	}
	//����
	if (num.size() != b.num.size())
		return num.size() < b.num.size();
	for (int i = num.size() - 1; i >= 0; i--)
		if (num[i] != b.num[i])
			return num[i] < b.num[i];
	return false;
}
Number Number::operator<<(const int a) const {
	Number res(this->sign, w + a, 0);
	int x = 0;
	int pos = a % WIDTH;
	int start = a / WIDTH;
	for (int i = 0; i < this->num.size(); i++) {
		res.num[start + i] = x | (this->num[i] << pos);
		x = this->num[i] >> (WIDTH - pos);
	}
	res.num[start + num.size()] = x;
	return res;
}
Number Number::operator>>(const int a) const {
	Number res(this->sign, w - a, 0);
	int x = 0;
	int pos = a % WIDTH;
	int start = a / WIDTH;
	for (int i = res.num.size(); i >= 0; i--) {
		res.num[i] = x | (this->num[start + i] >> pos);
		x = this->num[i] << (WIDTH - pos);
	}
	return res;
}

Number Number::operator&(const Number&b) const {
	Number res(this->sign, this->w, 0);
	for (int i = 0; i < b.num.size(); i++) {
		res.num[i] = this->num[i] & b.num[i];
	}
	return res;
}

Number Number::operator|(const Number&b) const {
	Number res(this->sign, this->w, 0);
	for (int i = 0; i < b.num.size(); i++) {
		res.num[i] = this->num[i] | b.num[i];
	}
	return res;
}

Number Number::operator^(const Number&b) const {
	Number res(this->sign, this->w, 0);
	for (int i = 0; i < b.num.size(); i++) {
		res.num[i] = this->num[i] ^ b.num[i];
	}
	return res;
}

Number Number::bits(int low, int high) const {
	Number res(this->sign, high - low + 1, 0);
	res = *this >> low;
	return res;
}

Number Number::cat(const Number & b) const {
	Number res(*this | b << this->w);
	return res;
}

Number Number::tail(int n) const {
	Number res(this->sign, this->w - n, 0);
	res = *this;
	return res;
}

Number Number::pad(int n) const {
	Number res(this->sign, this->w - n, 0);
	res = *this;
	return res;
}

bool Number::operator> (const Number &b) const { return b < *this; } 
bool Number::operator<=(const Number &b) const { return !(b < *this); }
bool Number::operator>=(const Number &b) const { return !(*this < b); }
bool Number::operator!=(const Number &b) const { return b < *this || *this < b; }
bool Number::operator==(const Number &b) const { return !(b < *this) && !(*this < b); }																		// �߼������
bool Number::operator||(const Number &b) const { return *this != 0 || b != 0; } 
bool Number::operator&&(const Number &b) const { return *this != 0 && b != 0; } 
bool Number::operator!() { return (bool)(*this == 0); }


std::ostream &operator<<(std::ostream &out, const Number &x)
{
	if (!x.sign)
		out << '-';
	out << x.num.back();
	for (int i = x.num.size() - 2; i >= 0; i--) {
		char buf[10];
		//��WIDTH��BASR�б仯,�˴�Ҫ�޸�Ϊ%0(WIDTH)d
		sprintf(buf, "%08x", x.num[i]);
		for (int j = 0; j < strlen(buf); j++)
			out << buf[j];
	}
	return out;
}
std::istream &operator>>(std::istream &in, Number &x)       
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