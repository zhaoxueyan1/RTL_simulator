#include"pch.h"
#include"Reg.h"


Reg::Reg(int id, int dtype, int nxtNum, int pre_num )
	:Node(id, dtype, pre_num, nxtNum)
{}
void Reg::setData()
{
	if (!this->Data)
		free(this->Data);
	this->Data = (Node::Gptr+((*this->preTbl)[0]))->GetData();
}