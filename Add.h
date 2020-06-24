#pragma once
#include"Comb.h"

class Add :protected Comb {
public:
	//static Add *ptr;
	Add(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {
	}
	void* GetData() {
		if (status==Comb::setFlag)
			return Data;
		status += 1;
		//void * data = malloc(dType);
		*(int*)this->Data = *(int*)((*Node::AllSet)[(*preTbl)[0]]->GetData()) + *(int*)((*Node::AllSet)[(*preTbl)[1]]->GetData());
		return Data;
	}
	
	//name1->name2
	
	//static Node * GetInstance() { // ±ßµÄ¹ØÏµ
	//	if (Add::ptr == nullptr) {
	//		ptr = new Add(2, 4, 1, 2);
	//		Add::ptr->preTbl->push_back(0);
	//		Add::ptr->preTbl->push_back(3);
	//		Add::ptr->nxtTbl->push_back(1);
	//		//Add::ptr->nxtTbl->push_back(2); 
	//	}
	//	return ptr;
	//}
	static Node * Create(int id, int dtype, int nxtNum, int pre_num,vector<int>* preTbl,vector<int>* nxtTbl) {
		Node * ptr = new Add(id,dtype,nxtNum,pre_num);
		ptr->preTbl = preTbl;
		ptr->nxtTbl = nxtTbl;
		return ptr;
	}
};

class Mul :protected Comb {
public:
	Mul(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {
	}
	void* GetData() {
		if (status == Comb::setFlag)
			return Data;
		status += 1;
		*(int*)Data = *(int*)((*Node::AllSet)[(*preTbl)[0]]->GetData()) + *(int*)((*Node::AllSet)[(*preTbl)[1]]->GetData());
		return Data;
	}

	//name1->name2
	static Node * Create(int id, int dtype, int nxtNum, int pre_num, vector<int>* preTbl, vector<int>* nxtTbl) {
		Node * ptr = new Mul(id, dtype, nxtNum, pre_num);
		ptr->preTbl = preTbl;
		ptr->nxtTbl = nxtTbl;
		return ptr;
	}
};
