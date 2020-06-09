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
	//Node::N++;
}

void Node::SetArg(int dType, int preNum, int nxtNum) {

	this->dType = dType;
	this->preNum = preNum;
	this->nxtNum = nxtNum;

	preTbl = new vector<int>;
	nxtTbl = new vector<int>;

}

void Node::setData(void * p) {
	if (!this->Data)
		free(this->Data);
	this->Data = p;
}

vector<Node*>* Node::AllSet = nullptr;
vector<Node*>* Node::CombSet = nullptr;
vector<Node*>* Node::RegSet = nullptr;

Node* Node::Gptr = nullptr;