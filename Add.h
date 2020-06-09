#pragma once
#include"Comb.h"

class Add :protected Comb {
public:
	static Add *ptr;
	Add(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {
	}
	void* GetData() {
		if (Data && status==Comb::setFlag)
			return Data;
		status += 1;
		void * data = malloc(dType);
		*(int*)data = *(int*)(Node::Gptr[(*preTbl)[0]].GetData()) + *(int*)(Node::Gptr[(*preTbl)[0]].GetData());
		return data;
	}
	
	//name1->name2
	
	static Node * GetInstance() { // ±ßµÄ¹ØÏµ
		if (Add::ptr == nullptr) {
			ptr = new Add(0, 4, 2, 1);
			Add::ptr->preTbl->push_back(1);
			Add::ptr->preTbl->push_back(1);
			Add::ptr->nxtTbl->push_back(1);
			Add::ptr->nxtTbl->push_back(2);
		}
		return ptr;
	}
};
