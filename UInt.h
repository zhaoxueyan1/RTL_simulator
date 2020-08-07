#pragma once
#include"pch.h"

struct UInt
{
	int w;
	// True  :postive , False :negtive
	std::vector<uint32_t> num;
	uint32_t mask;
	static const int WIDTH = 32;
	static const uint32_t BASE = 0Xffffffff;

	UInt(long long x);
	UInt(int w , long long x);
	UInt(const UInt &x);

	UInt& operator=(long long x);
	UInt& operator=(const std::string &str);
	UInt& operator=(const UInt &tmp);

	UInt abs() const;
	UInt operator+() const;
	//UInt operator-() const;
	UInt operator+(const UInt &b) const;
	UInt operator-(const UInt &b) const;
	UInt operator*(const UInt &b) const;
	UInt operator/(const UInt &b) const;
	UInt operator%(const UInt &b) const;

	bool opearator() const;
	
	UInt& operator++();
	UInt& operator--();
	UInt& operator+=(const UInt &b);
	UInt& operator-=(const UInt &b);
	UInt& operator*=(const UInt &b);
	UInt& operator/=(const UInt &b);
	UInt& operator%=(const UInt &b);

	UInt operator<<(const int a) const;
	UInt operator>>(const int a) const;
	UInt operator& (const UInt&b) const;
	UInt operator| (const UInt&b) const;
	UInt operator^ (const UInt&b) const;
	UInt operator~ () const;
	UInt bits(int low, int high) const;
	UInt cat(const UInt & b) const;
	UInt tail(int n) const;
	UInt pad(int n) const;

	bool operator< (const UInt &b) const;
	bool operator> (const UInt &b) const;
	bool operator<=(const UInt &b) const;
	bool operator>=(const UInt &b) const;
	bool operator!=(const UInt &b) const;
	bool operator==(const UInt &b) const;
	bool operator||(const UInt &b) const;
	bool operator&&(const UInt &b) const;
	bool operator! ();
	friend std::ostream &operator<<(std::ostream &out, const UInt &x);
	friend std::istream &operator>>(std::istream &in, UInt &x);
};

