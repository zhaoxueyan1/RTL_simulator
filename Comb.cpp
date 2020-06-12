#include"pch.h"
#include"Comb.h"

int Comb::setFlag = 1;

Comb::Comb(int id, int dtype, int pre_num, int nxtNum)
	:Node(id, dtype, pre_num, nxtNum)
{
	//Comb::CombNumber++;
}