#include"pch.h"
#include"UInt.h"

UInt::UInt(long long x)
{
	w = log2(x);
	*this = x;
	int t = w % (WIDTH + 1);
	mask = t == WIDTH ? BASE : (1 << t ) - 1;
}

UInt::UInt(int w, long long x)
	: w(w)
{
	*this = x;
	int t = w % (WIDTH + 1);
	mask = t == WIDTH ? BASE : (1 << t ) - 1;
}

UInt::UInt(const UInt &x) {
	this->w = x.w;
	this->mask = x.mask;
	for (int i = 0; i < ceil(1.*w / WIDTH) - num.size(); i++) {
		num.push_back(0);
	}
	*this = x;
}

UInt& UInt::operator=(long long x) {
	num.clear();
	do {
		num.push_back(x & BASE);
		x = x >> (WIDTH);
	} while (x > 0);
	for (int i = 0; i < ceil(1.*w / WIDTH) - num.size(); i++) {
		num.push_back(0);
	}
	return *this;
}

UInt& UInt::operator=(const std::string &str) {
	num.clear();
	int ww = WIDTH / 4;
	int x, len = ceil(1.* (str.size()) / ww);
	for (int i = 0; i < len; i++) {
		int End = str.size() - i * ww;
		int start = std::max(0, End - ww); //��ֹԽ��
		sscanf(str.substr(start, End - start).c_str(), "%x", &x);
		num[i] = x;
	}
	for (int i = 0; i < ceil(1.*w / WIDTH) - num.size(); i++) {
		num.push_back(0);
	}
	return *this;
}

UInt& UInt::operator=(const UInt &tmp) {
	if (this == &tmp) return *this;
	for (int i = 0; i < std::min(num.size(),tmp.num.size()); i++) {
		num[i] = tmp.num[i];
	}
	num[num.size() - 1] = num[num.size() - 1] & mask;
	return *this;
}

UInt UInt::abs() const {
	return *this;
}

UInt UInt::operator+() const { return *this; }

UInt UInt::operator+(const UInt &b) const {
	UInt ans(std::max(this->w, b.w)+1, 0);
	for (int i = 0, g = 0;; i++) {
		if (i >= num.size() && i >= b.num.size())
			break;
		int x = g;
		if (i < num.size())
			x += num[i];
		if (i < b.num.size())
			x += b.num[i];
		ans.num[i] = x & BASE;
		g = x >> WIDTH;
	}
	ans.num[ans.num.size() - 1] &= ans.mask;
	return ans;
}

UInt UInt::operator-(const UInt &b) const {
	UInt ans(std::max(this->w, b.w) + 1, 0);
	long long g = 0;
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
	ans.num[ans.num.size() - 1] &= ans.mask;
	return ans;
}

UInt UInt::operator*(const UInt &b) const {
	int lena = num.size(), lenb = b.num.size();
	UInt ans(this->w + b.w, 0);
	for (int i = 0, g = 0; i < lena; i++) {
		g = 0;
		for (int j = 0; j < lenb; j++) {
			long long x = ans.num[i + j] + g;
			x += (long long)num[i] * (long long)b.num[j];
			ans.num[i + j] = x & BASE;
			g = x >> WIDTH;
			//ans.num[i + j + 1] += g;
		}
	}
	ans.num[ans.num.size()-1] &= ans.mask;
	return ans;
}

//BUG
UInt UInt::operator/(const UInt &b) const {
	if (*this < b)
		return 0;
	UInt tmp(b.w+WIDTH, 0);
	UInt ans(w, 0);
	for (int i = b.num.size() - 2; i >= 0; i++) {
		tmp.num[i] = num[i];
	}
	int lena = this->num.size(), lenb = b.num.size();
	for (int i = 0; i <= lena - lenb; i++) {
		tmp.num[0] = this->num[lenb - 1 + i];
		long long t = 0;
		while (tmp >= b) {
			t++;
			tmp = tmp - b;
		}
		for (int i = tmp.num.size()-1; i > 0; i--) {
			tmp.num[i] = tmp.num[i - 1];
		} 
		ans.num[lena - lenb - i] = t;
	}
	ans.num[ans.num.size() - 1] &= ans.mask;
	return ans;
}

bool UInt::opearator() const {
	if (this->w == 1 && this->num[0] == 0) return false;
	if (this->w == 1 && this->num[0] != 0) return true;
	for (int i = 0; i < this->num.size(); i++) {
		if (this->num[i] != 0) {
			return true;
		}
	}
	return false;
}

UInt UInt::operator%(const UInt &b) const
{
	UInt bb(b.abs());
	UInt aa(this->abs());
	if (*this < bb)
		return 0;
	UInt tmp(bb.w + 1, 0);
	UInt ans(w, 0);
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
	return tmp;
}

UInt& UInt::operator++()
{
	*this = *this + 1;
	return *this;
}
UInt& UInt::operator--()
{
	*this = *this - 1;
	return *this;
}
UInt& UInt::operator+=(const UInt &b)
{
	*this = *this + b;
	return *this;
}
UInt& UInt::operator-=(const UInt &b)
{
	*this = *this - b;
	return *this;
}

UInt& UInt::operator*=(const UInt &b)
{
	*this = *this * b;
	return *this;
}

UInt& UInt::operator/=(const UInt &b)
{
	*this = *this / b;
	return *this;
}

UInt& UInt::operator%=(const UInt &b)
{
	*this = *this % b;
	return *this;
}

bool UInt::operator<(const UInt &b) const
{
	/*if (num.size() != b.num.size())
		return num.size() < b.num.size();*/
	for (int i = std::max(num.size(), b.num.size()) - 1; i >= 0; i--)
	{
		if (i<num.size() && i>=b.num.size()) {
			if (num[i] > 0) {
				return false;
			}
		}
		else if (i >= num.size()&&i < b.num.size()) {
			if (b.num[i] > 0) {
				return true;
			}
		}
		else {
			if(num[i]!=b.num[i])
				return num[i] < b.num[i];
		}
	}
	return false;
}
UInt UInt::operator<<(const int a) const {
	UInt res(w + a, 0);
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
UInt UInt::operator>>(const int a) const {
	UInt res(w - a, 0);
	int x = 0;
	int pos = a % WIDTH;
	int start = a / WIDTH;
	for (int i = res.num.size(); i >= 0; i--) {
		res.num[i] = x | (this->num[start + i] >> pos);
		x = this->num[i] << (WIDTH - pos);
	}
	return res;
}

UInt UInt::operator&(const UInt&b) const {
	UInt res(this->w, 0);
	for (int i = 0; i < b.num.size(); i++) {
		res.num[i] = this->num[i] & b.num[i];
	}
	return res;
}

UInt UInt::operator|(const UInt&b) const {
	UInt res(this->w, 0);
	for (int i = 0; i < b.num.size(); i++) {
		res.num[i] = this->num[i] | b.num[i];
	}
	return res;
}

UInt UInt::operator^(const UInt&b) const {
	UInt res(this->w, 0);
	for (int i = 0; i < b.num.size(); i++) {
		res.num[i] = this->num[i] ^ b.num[i];
	}
	return res;
}
UInt UInt::operator~() const {
	UInt res(this->w, 0);
	for (int i = 0; i < num.size(); i++) {
		res.num[i] = ~this->num[i];
	}
	res.num[num.size() - 1] = res.num[num.size() - 1] & mask;
	return res;
}

UInt UInt::bits(int low, int high) const {
	UInt res(high - low + 1, 0);
	res = *this >> low;
	return res;
}

UInt UInt::cat(const UInt & b) const {
	UInt res(*this | b << this->w);
	return res;
}

UInt UInt::tail(int n) const {
	UInt res(this->w - n, 0);
	res = *this;
	return res;
}

UInt UInt::pad(int n) const {
	UInt res(this->w - n, 0);
	res = *this;
	return res;
}

bool UInt::operator> (const UInt &b) const { return b < *this; }
bool UInt::operator<=(const UInt &b) const { return !(b < *this); }
bool UInt::operator>=(const UInt &b) const { return !(*this < b); }
bool UInt::operator!=(const UInt &b) const { return b < *this || *this < b; }
bool UInt::operator==(const UInt &b) const { return !(b < *this) && !(*this < b); }																		// �߼������
bool UInt::operator||(const UInt &b) const { return *this != 0 || b != 0; }
bool UInt::operator&&(const UInt &b) const { return *this != 0 && b != 0; }
bool UInt::operator!() { return (bool)(*this == 0); }


std::ostream &operator<<(std::ostream &out, const UInt &x)
{
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

std::istream &operator>>(std::istream &in, UInt &x)
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
	//x = str.c_str();
	return in;
}