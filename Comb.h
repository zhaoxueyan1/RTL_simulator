#pragma once
#include "pch.h"
#include "Node.h"

class Comb :public Node 
{
public:
	static bool setFlag;
	static int CombNumber;
	Comb() {
	}
	Comb(int id, int dtype, int pre_num, int nxtNum);

};


