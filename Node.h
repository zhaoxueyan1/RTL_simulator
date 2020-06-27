#pragma once
#include"pch.h"
#include"Type.h"

using namespace std;

class Node {

public:
	void * Data; // the data of this register

public:
	Node() {}
	
	Node(int id, int dType, int nxtNum, int preNum);
	
	//static int cntComb; // the number of Comb
	//static int cntReg;  // the number of Reg
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
	
	virtual void Update() = 0;
	virtual void SetData(void * Data) {
		if (this->Data)
			free(this);
		this->Data = Data;
	}
	virtual UINT_64 getUIntData() { return 0; }
	virtual SINT_64 getSIntData() { return 0; }
	// virtual void* Create() = 0;
	//void setData(UINT_64 b);
	void* getData() {
		return Data;
	}
	virtual ~Node() {}
};
