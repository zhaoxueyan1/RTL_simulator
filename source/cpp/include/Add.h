#pragma once
#include"Comb.h"

//class Add :public Comb {
//public:
//	Add(int id, int dtype, int nxtNum, int pre_num)
//		:Comb(id, dtype, pre_num, nxtNum) {}
//	void Update() {
//		if (status == Comb::setFlag)
//			return ;
//		status += 1;
//		//void * data = malloc(dType);
//		*(UInt*)this->Data = *(UInt*)((*Node::AllSet)[0]->Data) + *(UInt*)((*Node::AllSet)[0]->Data);
//		return ;
//	}
//
//	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {
//		Node * ptr = new Add(id, dtype, nxtNum, pre_num);
//		ptr->Data = new UInt();
//		return ptr;
//	}
//};

# if 0
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
#endif