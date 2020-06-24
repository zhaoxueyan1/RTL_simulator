#pragma once
#include"pch.h"
#include"Type.h"
template<int w>
class Sint:public Type<w>
{
public:
	Sint();
	Sint(int val);
	~Sint();
	operator int();
};

