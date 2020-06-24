#pragma once
#include"pch.h"
//#include"Type.cpp"

template<int w>
class Type
{
public:
	Type();
	~Type();
	inline static int cal(int w) {
		return w <= 8 ? 1 : w <= 16 ? 2 : w <= 32 ? 4 : w <= 64 ? 8 : 8;
	}
	int t;
	void *data;
};

template<int w>
Type<w>::~Type()
{
	free(this->data);
}
template<int w>
Type<w>::Type()
{
	this->t = cal(w);
	this->data = malloc(t);
	memset(this->data, 0, this->t);
}
