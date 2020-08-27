#pragma once
#include"pch.h"
#include"UInt.h"
#include"Sint.h"

using namespace std;

class Node {

public:
	void * Data; // the data of this register

public:
	typedef std::function<void(void)>Func;
	
	Node() {}
	
	Node(int id, bool sign, Func func);
	

	// the index of this
	int id;  

	// the type of this Data
	bool sign;

	//Is this Node register
	enum NodeType
	{
		Reg,Comb
	};
	NodeType Type;
	
/*
	// describe the setflag
	int status=0; 


	// the number of the pre_node
	int preNum; 

	// the pre table
	vector<int>* preTbl=nullptr; 
	
	// the number of the next_node
	int nxtNum; 

	// the next table
	vector<int>* nxtTbl=nullptr; 
*/
	void SetArg(bool sign, int preNum, int nxtNum);

	static int N;

	// to storge the map to id-Node*
	static vector<Node*>* AllSet;  
	static vector<Node*>* CombSet;
	static vector<Node*>* RegSet;

	// UpdateFunc
	Func Updatefunc;

	inline void Update() {
		Updatefunc();
	}

	void SetData(long long x) {
		if (sign) {
			(*getSInt()) = x;
		}
		else {
			(*getUInt()) = x;
		}
	}
	
	UInt* getUInt() {
		return (UInt *)Data;
	}
	SInt* getSInt() {
		return (SInt *)Data;
	}

	~Node() {
		if (!Data) {
			free(Data);
		}
	}
};
