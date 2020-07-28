#pragma once
#include"pch.h"
#include<vector>
#include<string>

struct Number
{
	int w;
	// True  :postive , False :negtive
	bool sign;
	// 选用64位和32位的问题，主要考虑的是CPU种寄存器有64位，可能仿真时候直接进行64位加法
	std::vector<uint32_t> num;
	uint32_t mask;
	static const int WIDTH = 32;// 每位最多
	static const uint32_t BASE = 0Xffffffff;

	Number(bool sign = true, int w = 32, long long x = 0)
		:sign(sign), w(w)
	{
		*this = x;
		int t = w % (WIDTH+1);
		mask = t==WIDTH?BASE:(1<<t+1)-1;
	}

	Number(const Number &x) { *this = x; }
	
	Number &operator=(long long x) {
		sign = x > 0 ? true : false;
		num.clear();
		do {
			num.push_back(x & BASE);
			x >>(WIDTH);
		} while (x > 0);
		for (int i = 0; i <ceil(1.*w/WIDTH)-num.size() ; i++) {
			num.push_back(0);
		}
		return *this;
	}
	//Has a bug
	Number &operator=(const std::string &str) {
		num.clear();
		sign = (str[0] != '-'); //设置符号
		int ww = WIDTH / 4;
		int x, len = ceil(1.* (str.size() - 1 - (!sign)) / ww);
		for (int i = 0; i < len; i++) {
			int End = str.size() - i * ww;
			int start = std::max((int)(!sign), End - ww); //防止越界
			sscanf(str.substr(start, End - start).c_str(), "%x", &x);
			num[i] = x;
		}
		for (int i = 0; i < ceil(1.*w / WIDTH) - num.size(); i++) {
			num.push_back(0);
		}
		return *this;
	}

	Number &operator=(const Number &tmp) {
		w = tmp.w;
		num = tmp.num;
		sign = tmp.sign;
		w = tmp.w;
		return *this;
	}

	Number abs() const {
		Number ans(*this);
		ans.sign = false;
		return ans;
	}
	
	const Number &operator+() const { return *this; }
	
	Number operator-() const
	{
		Number ans(*this);
		if (ans != 0)
			ans.sign = !ans.sign;
		return ans;
	}
	
	Number operator+(const Number &b) const {
		if (!b.sign) {
			return *this - (-b);
		}
		if (!sign) {
			return b - (-*this);
		}
		Number ans(false,std::max(this->w,b.w),0);
		for (int i = 0, g = 0;; i++) {
			if (g == 0 && i >= num.size() && i >= b.num.size())
				break;
			int x = g;
			if (i < num.size())
				x += num[i];
			if (i < b.num.size())
				x += b.num[i];
			ans.num[i] = x & BASE;
			g = x >>WIDTH;
		}
		return ans;
	}
	
	// - 运算符
	Number operator-(const Number &b) const {
		Number ans(false,std::max(this->w,b.w)+1,0);
		if (b.sign) {
			return *this + (-b);
		}
		if (sign) {
			return -((-*this) + b);
		}
		if ((*this)<b) {
			/*return -(b - *this);*/
			ans.sign = false;
			for (int i = 0, long long g = 0;; i++) {
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
			for (int i = 0, long long g = 0;; i++) {
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

	// * 运算符
	Number operator*(const Number &b) const {
		int lena = num.size(), lenb = b.num.size();
		Number ans(false,this->w+b.w,0);
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
		ans.sign = !(sign==b.sign);
		return ans;
	}
	////*10^n 大数除大数中用到
	//Number e(size_t n) const
	//{
	//	int tmp = n % WIDTH;
	//	Number ans;
	//	ans.length = n + 1;
	//	n /= WIDTH;
	//	while (ans.num.size() <= n)
	//		ans.num.push_back(0);
	//	ans.num[n] = 1;
	//	while (tmp--)
	//		ans.num[n] *= 10;
	//	return ans * (*this);
	//}
	// 运算符 (大数除大数)
	
	// bug
	Number operator/(const Number &b) const
	{
		Number aa((*this).abs());
		Number bb(b.abs());
		if (aa < bb)
			return 0;
		char *str = new char[ceil(1.*aa.w/4)];
		Number tmp(aa.sign,aa.w,0);
		int lena = aa.w, lenb = bb.w;
		for (int i = 0; i <= lena - lenb; i++)
		{
			tmp = 0;
			while (aa >= tmp)
			{
				str[i]++;
				aa = aa - tmp;
			}
			str[i] += '0';
		}
		Number ans(str);
		delete[] str;
		ans.sign = (ans == 0 || sign == b.sign);
		return ans;
	}
	// %运算符
	bool opearator() const {
		return 1;
	}
	//bug
	Number operator%(const Number &b) const
	{
		Number aa((*this).abs());
		Number bb(b.abs());
		if (aa < bb)
			return 0;
		char *str = new char[ceil(1.*aa.w / 4)];
		Number tmp(aa.sign, aa.w, 0);
		int lena = aa.w, lenb = bb.w;
		for (int i = 0; i <= lena - lenb; i++)
		{
			tmp = 0;
			while (aa >= tmp)
			{
				str[i]++;
				aa = aa - tmp;
			}
			str[i] += '0';
		}
		Number ans(str);
		delete[] str;
		ans.sign = (ans == 0 || sign == b.sign);
		return ans;
	}
	// ++ 运算符
	Number &operator++()
	{
		*this = *this + 1;
		return *this;
	}
	// -- 运算符
	Number &operator--()
	{
		*this = *this - 1;
		return *this;
	}
	// += 运算符
	Number &operator+=(const Number &b)
	{
		*this = *this + b;
		return *this;
	}
	// -= 运算符
	Number &operator-=(const Number &b)
	{
		*this = *this - b;
		return *this;
	}
	// *=运算符
	Number &operator*=(const Number &b)
	{
		*this = *this * b;
		return *this;
	}
	// /= 运算符
	Number &operator/=(const Number &b)
	{
		*this = *this / b;
		return *this;
	}
	// %=运算符
	Number &operator%=(const Number &b)
	{
		*this = *this % b;
		return *this;
	}
	// < 运算符
	bool operator<(const Number &b) const
	{
		if (sign != b.sign) //正负，负正
		{
			return !sign;
		}
		else if (!sign && !b.sign) //负负
		{
			return -b < -*this;
		}
		//正正
		if (num.size() != b.num.size())
			return num.size() < b.num.size();
		for (int i = num.size() - 1; i >= 0; i--)
			if (num[i] != b.num[i])
				return num[i] < b.num[i];
		return false;
	}
	bool operator>(const Number &b) const { return b < *this; }                     // >  运算符
	bool operator<=(const Number &b) const { return !(b < *this); }                 // <= 运算符
	bool operator>=(const Number &b) const { return !(*this < b); }                 // >= 运算符
	bool operator!=(const Number &b) const { return b < *this || *this < b; }       // != 运算符
	bool operator==(const Number &b) const { return !(b < *this) && !(*this < b); } //==运算符
	// 逻辑运算符
	bool operator||(const Number &b) const { return *this != 0 || b != 0; } // || 运算符
	bool operator&&(const Number &b) const { return *this != 0 && b != 0; } // && 运算符
	bool operator!() { return (bool)(*this == 0); }                             // ！ 运算符

	//重载<<使得可以直接输出大数
	friend std::ostream &operator<<(std::ostream &out, const Number &x)
	{
		if (!x.sign)
			out << '-';
		out << x.num.back();
		for (int i = x.num.size() - 2; i >= 0; i--)
		{
			char buf[10];
			//如WIDTH和BASR有变化,此处要修改为%0(WIDTH)d
			sprintf(buf, "%08d", x.num[i]);
			for (int j = 0; j < strlen(buf); j++)
				out << buf[j];
		}
		return out;
	}



	//重载>>使得可以直接输入大数
	friend std::istream &operator>>(std::istream &in, Number &x)
	{
		std::string str;
		in >> str;
		size_t len = str.size();
		int start = 0;
		if (str[0] == '-')
			start = 1;
		if (str[start] == '\0')
			return in;
		for (int i = start; i < len; i++)
		{
			if (str[i] < '0' || str[i] > '9')
				return in;
		}
		x.sign = !start;
		//x = str.c_str();
		return in;
	}
	Number& bits(int low, int high) {

	}

};

class SInt :public Number {
	SInt(int w=32,int x=0) 
		:Number(true,w,x)
	{}
};

class UInt :public Number {
	UInt(int w = 32, int x = 0)
		:Number(true, w, x)
	{}
};