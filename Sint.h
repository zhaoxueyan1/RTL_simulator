#pragma once
#include"pch.h"
#include<vector>
#include<string>

struct Literal 
{
	static const int BASE = 100000000; 
	static const int WIDTH = 8;        
	int w;
	bool sign;                         
	size_t length;                     
	std::vector<int> num;              
									   
	Literal(bool sign=0, int w=32, long long x = 0)
		:sign(sign),w(w)
	{ *this = x; }
	Literal(const Literal &x){ *this = x; }
	void cutLeadingZero()
	{
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
	Literal &operator=(long long x)
	{
		num.clear();
		if (x >= 0)
		{
			sign = true;
		}
		else
		{
			sign = false;
			x = -x;
		}
		do
		{
			num.push_back(x % BASE);
			x /= BASE;
		} while (x > 0);
		cutLeadingZero();
		return *this;
	}
	Literal &operator=(const std::string &str)
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
	Literal &operator=(const Literal &tmp)
	{
		w = tmp.w;
		num = tmp.num;
		sign = tmp.sign;
		length = tmp.length;
		return *this;
	}
	//绝对值
	Literal abs() const
	{
		Literal ans(*this);
		ans.sign = true;
		return ans;
	}
	//正号
	const Literal &operator+() const { return *this; }
	//负号
	Literal operator-() const
	{
		Literal ans(*this);
		if (ans != 0)
			ans.sign = !ans.sign;
		return ans;
	}
	// + 运算符
	Literal operator+(const Literal &b) const
	{
		if (!b.sign)
		{
			return *this - (-b);
		}
		if (!sign)
		{
			return b - (-*this);
		}
		Literal ans(this->w);
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
	Literal operator-(const Literal &b) const
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
		Literal ans;
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
	Literal operator*(const Literal &b) const
	{
		int lena = num.size(), lenb = b.num.size();
		Literal ans;
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
	Literal e(size_t n) const
	{
		int tmp = n % WIDTH;
		Literal ans;
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
	Literal operator/(const Literal &b) const
	{
		Literal aa((*this).abs());
		Literal bb(b.abs());
		if (aa < bb)
			return 0;
		char *str = new char[aa.length + 1];
		memset(str, 0, sizeof(char) * (aa.length + 1));
		Literal tmp;
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
		Literal ans(str);
		delete[] str;
		ans.sign = (ans == 0 || sign == b.sign);
		return ans;
	}
	// %运算符
	Literal operator%(const Literal &b) const
	{
		return *this - *this / b * b;
	}
	// ++ 运算符
	Literal &operator++()
	{
		*this = *this + 1;
		return *this;
	}
	// -- 运算符
	Literal &operator--()
	{
		*this = *this - 1;
		return *this;
	}
	// += 运算符
	Literal &operator+=(const Literal &b)
	{
		*this = *this + b;
		return *this;
	}
	// -= 运算符
	Literal &operator-=(const Literal &b)
	{
		*this = *this - b;
		return *this;
	}
	// *=运算符
	Literal &operator*=(const Literal &b)
	{
		*this = *this * b;
		return *this;
	}
	// /= 运算符
	Literal &operator/=(const Literal &b)
	{
		*this = *this / b;
		return *this;
	}
	// %=运算符
	Literal &operator%=(const Literal &b)
	{
		*this = *this % b;
		return *this;
	}
	// < 运算符
	bool operator<(const Literal &b) const
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
	bool operator>(const Literal &b) const { return b < *this; }                     // >  运算符
	bool operator<=(const Literal &b) const { return !(b < *this); }                 // <= 运算符
	bool operator>=(const Literal &b) const { return !(*this < b); }                 // >= 运算符
	bool operator!=(const Literal &b) const { return b < *this || *this < b; }       // != 运算符
	bool operator==(const Literal &b) const { return !(b < *this) && !(*this < b); } //==运算符
	// 逻辑运算符
	bool operator||(const Literal &b) const { return *this != 0 || b != 0; } // || 运算符
	bool operator&&(const Literal &b) const { return *this != 0 && b != 0; } // && 运算符
	bool operator!() { return (bool)(*this == 0); }                             // ！ 运算符

	//重载<<使得可以直接输出大数
	friend std::ostream &operator<<(std::ostream &out, const Literal &x)
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
	friend std::istream &operator>>(std::istream &in, Literal &x)
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
