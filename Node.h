#pragma once
#include"pch.h"

using namespace std;

class Node {

protected:
	void * Data; // the data of this register

public:
	Node() {}
	
	Node(int id, int dType, int nxtNum, int preNum);
	
	int cntComb; // the number of Comb
	int cntReg;  // the number of Reg
	int id; // the index of this 
	int dType;// the type of this register
	int status=0; // describe the setflag
	int preNum; // the number of the pre_node
	vector<int>* preTbl=nullptr; // the pre table
	int nxtNum; // the number of the next_node
	vector<int>* nxtTbl=nullptr; // the next table
	
	void SetArg(int dType, int preNum, int nxtNum);

	static int N;
	static vector<Node*>* AllSet;  // to storge the map to id-Node*
	static vector<Node*>* CombSet;
	static vector<Node*>* RegSet;
	static Node * Gptr; // to storge the whole objects
	
	virtual void* GetData() = 0;

	virtual void setData(void * p);

	virtual ~Node() {}
};