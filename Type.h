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
	UINT_64 data;
	virtual void setData(UINT_64 b) = 0;
};

template<int w>
Type<w>::~Type()
{
	
}
template<int w>
Type<w>::Type()
{
	this->t = CALC(w);
	this->t = 0;
}
