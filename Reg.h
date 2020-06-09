#pragma once
#include"pch.h"
#include"Node.h"

class Reg :public Node
{
public:
	Reg(int id, int dtype, int nxtNum, int pre_num = 1);
	~Reg() {
	}
	static int regNumber;
	void* getData() {
		return this->Data;
	}
	void setData();
};