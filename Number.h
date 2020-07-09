#pragma once
#include"pch.h"
#include<vector>
#include<string>

struct Number
{
	int w;
	bool sign;
	size_t length;
	std::vector<int> num;
	static const int WIDTH = 8;
	static const int BASE = 1000000000;
	Number(bool sign = 0, int w = 32, long long x = 0)
		:sign(sign), w(w)
	{
		*this = x;
	}
	Number(const Number &x) { *this = x; }
	void cutLeadingZero() {
		while (num.back() == 0 && num.size() != 1)
		{
			num.pop_back();
		}
		int tmp = num.back();
		if (tmp == 0)
		{
			length = 1;
		}
		else
		{
			length = (num.size() - 1) * WIDTH;
			while (tmp > 0)
			{
				length++;
				tmp /= 10;
			}
		}
	}
	Number &operator=(long long x)
	{
		num.clear();
		if (x >= 0)	{
			sign = true;
		}
		else{
			sign = false;
			x = -x;
		}
		do{
			num.push_back(x % BASE);
			x /= BASE;
		} while (x > 0);
		cutLeadingZero();
		return *this;
	}
	Number &operator=(const std::string &str)
	{
		num.clear();
		sign = (str[0] != '-'); //设置符号
		int x, len = (str.size() - 1 - (!sign)) / WIDTH + 1;
		for (int i = 0; i < len; i++)
		{
			int End = str.size() - i * WIDTH;
			int start = std::max((int)(!sign), End - WIDTH); //防止越界
			sscanf(str.substr(start, End - start).c_str(), "%d", &x);
			num.push_back(x);
		}
		cutLeadingZero();
		return *this;
	}
	Number &operator=(const Number &tmp)
	{
		w = tmp.w;
		num = tmp.num;
		sign = tmp.sign;
		length = tmp.length;
		return *this;
	}
	//绝对值
	Number abs() const
	{
		Number ans(*this);
		ans.sign = true;
		return ans;
	}
	//正号
	const Number &operator+() const { return *this; }
	//负号
	Number operator-() const
	{
		Number ans(*this);
		if (ans != 0)
			ans.sign = !ans.sign;
		return ans;
	}
	// + 运算符
	Number operator+(const Number &b) const
	{
		if (!b.sign)
		{
			return *this - (-b);
		}
		if (!sign)
		{
			return b - (-*this);
		}
		Number ans(this->w);
		ans.num.clear();
		for (int i = 0, g = 0;; i++)
		{
			if (g == 0 && i >= num.size() && i >= b.num.size())
				break;
			int x = g;
			if (i < num.size())
				x += num[i];
			if (i < b.num.size())
				x += b.num[i];
			ans.num.push_back(x % BASE);
			g = x / BASE;
		}
		ans.cutLeadingZero();
		return ans;
	}
	// - 运算符
	Number operator-(const Number &b) const
	{
		if (!b.sign)
		{
			return *this + (-b);
		}
		if (!sign)
		{
			return -((-*this) + b);
		}
		if (*this < b)
		{
			return -(b - *this);
		}
		Number ans;
		ans.num.clear();
		for (int i = 0, g = 0;; i++)
		{
			if (g == 0 && i >= num.size() && i >= b.num.size())
				break;
			int x = g;
			g = 0;
			if (i < num.size())
				x += num[i];
			if (i < b.num.size())
				x -= b.num[i];
			if (x < 0)
			{
				x += BASE;
				g = -1;
			}
			ans.num.push_back(x);
		}
		ans.cutLeadingZero();
		return ans;
	}
	// * 运算符
	Number operator*(const Number &b) const
	{
		int lena = num.size(), lenb = b.num.size();
		Number ans;
		for (int i = 0; i < lena + lenb; i++)
			ans.num.push_back(0);
		for (int i = 0, g = 0; i < lena; i++)
		{
			g = 0;
			for (int j = 0; j < lenb; j++)
			{
				long long x = ans.num[i + j];
				x += (long long)num[i] * (long long)b.num[j];
				ans.num[i + j] = x % BASE;
				g = x / BASE;
				ans.num[i + j + 1] += g;
			}
		}
		ans.cutLeadingZero();
		ans.sign = (ans.length == 1 && ans.num[0] == 0) || (sign == b.sign);
		return ans;
	}
	//*10^n 大数除大数中用到
	Number e(size_t n) const
	{
		int tmp = n % WIDTH;
		Number ans;
		ans.length = n + 1;
		n /= WIDTH;
		while (ans.num.size() <= n)
			ans.num.push_back(0);
		ans.num[n] = 1;
		while (tmp--)
			ans.num[n] *= 10;
		return ans * (*this);
	}
	// /运算符 (大数除大数)
	Number operator/(const Number &b) const
	{
		Number aa((*this).abs());
		Number bb(b.abs());
		if (aa < bb)
			return 0;
		char *str = new char[aa.length + 1];
		memset(str, 0, sizeof(char) * (aa.length + 1));
		Number tmp;
		int lena = aa.length, lenb = bb.length;
		for (int i = 0; i <= lena - lenb; i++)
		{
			tmp = bb.e(lena - lenb - i);
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
	Number operator%(const Number &b) const
	{
		return *this - *this / b * b;
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
};
