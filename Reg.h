#pragma once
#include"pch.h"
#include"Node.h"
#include"Uint.h"

class Reg :public Node
{
public:
	Reg(int id, int dtype, int nxtNum, int pre_num = 1);
	~Reg() {
	}
	
	virtual void Update();
};

Reg::Reg(int id, int dtype, int nxtNum, int pre_num)
	:Node(id, dtype, pre_num, nxtNum)
{}
void Reg::Update()
{
	/*if (!this->Data)
		free(this->Data);*/
	memcpy(this->Data, (*Node::AllSet)[(*this->preTbl)[0]]->Data, dType);
}

class Reg1 :public Reg
{
public:
	Reg1(int id, int dtype, int nxtNum, int pre_num = 1) 
		:Reg(id,dtype, nxtNum,pre_num){}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	//SINT_64 getSIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) { Node * ptr = new Reg1(id, dtype, nxtNum, 1);
		ptr->Data = new UInt<32>(0);
		(*ptr->preTbl).push_back(1);
		return ptr;
	}
};
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
