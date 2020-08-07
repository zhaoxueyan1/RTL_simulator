#pragma once
#include"pch.h"
#include"UInt.h"

struct SInt
{
	// True  :postive , False :negtive
	bool sign;
	UInt* pnum;

	SInt(long long x, bool sign = true);
	SInt(long long x , int w , bool sign = true);
	SInt(const SInt &x);
	~SInt();
	SInt& operator=(long long x);
	SInt& operator=(const std::string &str);
	SInt& operator=(const SInt &str);

	SInt abs() const;
	SInt operator+() const;
	SInt operator-() const;
	SInt operator+(const SInt &b) const;
	SInt operator-(const SInt &b) const;
	SInt operator*(const SInt &b) const;
	SInt operator/(const SInt &b) const;
	SInt operator%(const SInt &b) const;

	bool opearator() const;

	SInt& operator++();
	SInt& operator--();
	SInt& operator+=(const SInt &b);
	SInt& operator-=(const SInt &b);
	SInt& operator*=(const SInt &b);
	SInt& operator/=(const SInt &b);
	SInt& operator%=(const SInt &b);

	SInt operator<<(const int a) const;
	SInt operator~() const;
	SInt operator>>(const int a) const;
	SInt operator& (const SInt&b) const;
	SInt operator| (const SInt&b) const;
	SInt operator^ (const SInt&b) const;
	SInt bits(int low, int high) const;
	SInt cat(const SInt & b) const;
	SInt tail(int n) const;
	SInt pad(int n) const;
	bool operator< (const SInt &b) const;
	bool operator> (const SInt &b) const;
	bool operator<=(const SInt &b) const;
	bool operator>=(const SInt &b) const;
	bool operator!=(const SInt &b) const;
	bool operator==(const SInt &b) const;
	bool operator||(const SInt &b) const;
	bool operator&&(const SInt &b) const;
	void absTocom();
	bool operator! ();
	friend std::ostream &operator<<(std::ostream &out, const SInt &x);
	friend std::istream &operator>>(std::istream &in, SInt &x);

	bool setflag();
};
