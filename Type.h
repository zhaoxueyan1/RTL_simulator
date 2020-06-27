#pragma once
#include"pch.h"
//#include"Type.cpp"

template<int w>
class Type
{
public:
	Type();
	~Type();
	int t;
	//virtual operator=(UINT_64 b)=0;
	void *data;
	virtual void setData(UINT_64 b) = 0;
};

template<int w>
Type<w>::~Type()
{
	free(this->data);
}
template<int w>
Type<w>::Type()
{
	this->t = CALC(w);
	this->data = malloc(t);
	memset(this->data, 0, this->t);
}
