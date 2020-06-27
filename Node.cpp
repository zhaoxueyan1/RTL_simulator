#pragma once
#include"pch.h"
#include"Node.h"
using namespace std;
//#include"GenerateNode.h"


Node::Node(int id, int dType, int nxtNum, int preNum)
	:id(id), dType(dType), nxtNum(nxtNum), preNum(preNum)
{
	preTbl = new vector<int>;
	nxtTbl = new vector<int>;
	//this->Data = malloc(dType);
	//memset(this->Data, 0, dType);
	//Node::N++;
}

void Node::SetArg(int dType, int preNum, int nxtNum) {

	this->dType = dType;
	this->preNum = preNum;
	this->nxtNum = nxtNum;

	preTbl = new vector<int>;
	nxtTbl = new vector<int>;

}

//void Node::setData(UINT_64 b) {
//	((Type<32> *)this->Data)->setData(b);
//}

vector<Node*>* Node::AllSet = nullptr;
vector<Node*>* Node::CombSet = nullptr;
vector<Node*>* Node::RegSet = nullptr;

Node* Node::Gptr = nullptr;