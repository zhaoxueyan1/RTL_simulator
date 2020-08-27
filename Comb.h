#pragma once
#if 0
#include "pch.h"
#include "Node.h"

class Comb :public Node 
{
public:
	static int CombNumber;
	Comb() {
	}
	Comb(int id, int dtype, Func UpdateFunc);
};

Comb::Comb(int id, int dtype, Func UpdateFunc)
	:Node(id, dtype, UpdateFunc)
{
	//Comb::CombNumber++;
}
#endif