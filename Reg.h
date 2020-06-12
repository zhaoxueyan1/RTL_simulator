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
	void* GetData() {
		return this->Data;
	}
	void setData();
	static Node * Create(int id, int dtype, int nxtNum, int pre_num, vector<int>* preTbl, vector<int>* nxtTbl) {
		Node * ptr = new Reg(id, dtype, nxtNum, pre_num);
		ptr->preTbl = preTbl;
		ptr->nxtTbl = nxtTbl;
		return ptr;
	}
};

//class Reg1 :public Reg
//{
//public:
//	Reg1(int id, int dtype, int nxtNum, int pre_num = 1);
//	~Reg1() {
//	}
//	static int regNumber;
//	void* GetData() {
//		return this->Data;
//	}
//	void setData();
//};
//
//class Reg1 :public Node
//{
//public:
//	Reg1(int id, int dtype, int nxtNum, int pre_num = 1);
//	~Reg1() {
//	}
//	static int regNumber;
//	void* GetData() {
//		return this->Data;
//	}
//	void setData();
//};
//
// 八路计算八位门，四位计算
