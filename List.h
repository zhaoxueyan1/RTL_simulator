#pragma once
#include"Comb.h"
#include"Uint.h"
#include"Reg.h"
class Node7 :public Comb {
public:
	Node7(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {}
	void* GetData() { if (status==Comb::setFlag) return Data; 
		status += 1;
		//Uint<32>* a = new Uint<32>(2);
		*(UInt<32>*)this->Data =  *(UInt<32>*)((*Node::AllSet)[5]->Data);
		
		return Data;
}
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Node7 (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32>();
		return ptr;
}
};

class Node8 :public Comb {
public:
	Node8(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {}
	void* GetData() { if (status==Comb::setFlag) return Data; status += 1;
		*(UInt<1> *)this->Data =  (*(UInt<32>*)((*Node::AllSet)[6]->Data) == UInt<1>(0x0));
		return Data;
}
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Node8 (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<1> ();
		return ptr;
}
};

class Node9 :public Comb {
public:
	Node9(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {}
	void* GetData() { if (status==Comb::setFlag) return Data; status += 1;
		*(UInt<32> *)this->Data =  (*(UInt<1>*)((*Node::AllSet)[2]->Data) ? *(UInt<32>*)((*Node::AllSet)[0]->Data) : ((*(UInt<32>*)((*Node::AllSet)[5]->Data) > *(UInt<32>*)((*Node::AllSet)[6]->Data)) ? ((((*(UInt<32>*)((*Node::AllSet)[5]->Data) - *(UInt<32>*)((*Node::AllSet)[6]->Data)) + *(UInt<32>*)((*Node::AllSet)[6]->Data)) - *(UInt<32>*)((*Node::AllSet)[6]->Data)).tail<1>()) : *(UInt<32>*)((*Node::AllSet)[5]->Data))).bits<31,0>();
		return Data;
}
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Node9 (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		return ptr;
}
};

class Node10 :public Comb {
public:
	Node10(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {}
	void* GetData() { if (status==Comb::setFlag) return Data; status += 1;
		*(UInt<32> *)this->Data =  (*(UInt<1>*)((*Node::AllSet)[2]->Data) ? *(UInt<32>*)((*Node::AllSet)[1]->Data) : (((*(UInt<32>*)((*Node::AllSet)[5]->Data) > *(UInt<32>*)((*Node::AllSet)[6]->Data)) == UInt<1>(0x0)) ? ((*(UInt<32>*)((*Node::AllSet)[6]->Data) - *(UInt<32>*)((*Node::AllSet)[5]->Data)).tail<1>()) : *(UInt<32>*)((*Node::AllSet)[6]->Data)));
		return Data;
}
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Node10 (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		return ptr;
}
};

namespace Generate {
	void GenerateNode() {
		vector<int> *p = new vector<int>;
		p->push_back( 0);
		(*Node::AllSet).push_back(Reg::Create(0,sizeof(UInt<32>),0,2,p));
		p->clear();
		p->push_back( 1);
		(*Node::AllSet).push_back(Reg::Create(1,sizeof(UInt<32>),0,2,p));
		p->clear();
		p->push_back( 2);
		(*Node::AllSet).push_back(Reg::Create(2,sizeof(UInt<1>),0,2,p));
		p->clear();
		p->push_back( 7);
		(*Node::AllSet).push_back(Reg::Create(3,sizeof(UInt<32>),0,2,p));
		p->clear();
		p->push_back( 8);
		(*Node::AllSet).push_back(Reg::Create(4,sizeof(UInt<1>),0,2,p));
		p->clear();
		p->push_back( 9);
		(*Node::AllSet).push_back(Reg::Create(5,sizeof(UInt<32>),0,2,p));
		p->clear();
		p->push_back( 10);
		(*Node::AllSet).push_back(Reg::Create(6,sizeof(UInt<32>),0,2,p));
		p->clear();
		(*Node::AllSet).push_back(Node7::Create(7,sizeof(UInt<32>),0,2));
		p->clear();
		(*Node::AllSet).push_back(Node8::Create(8,sizeof(UInt<1>),0,2));
		p->clear();
		(*Node::AllSet).push_back(Node9::Create(9,sizeof(UInt<32>),0,2));
		p->clear();
		(*Node::AllSet).push_back(Node10::Create(10,sizeof(UInt<32>),0,2));
	}
}