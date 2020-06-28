#pragma once
#include"Comb.h"
#include"Uint.h"
#include"Reg.h"
class Regio_a :public Reg {
public:
	Regio_a(int id, int dtype, int nxtNum, int pre_num)
		:Reg(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Regio_a (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		(*ptr->preTbl).push_back( 0);
		return ptr;
}
};
class Regio_b :public Reg {
public:
	Regio_b(int id, int dtype, int nxtNum, int pre_num)
		:Reg(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Regio_b (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		(*ptr->preTbl).push_back( 1);
		return ptr;
}
};
class Regio_e :public Reg {
public:
	Regio_e(int id, int dtype, int nxtNum, int pre_num)
		:Reg(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<1> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Regio_e (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<1> ();
		(*ptr->preTbl).push_back( 2);
		return ptr;
}
};
class Regio_z :public Reg {
public:
	Regio_z(int id, int dtype, int nxtNum, int pre_num)
		:Reg(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Regio_z (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		(*ptr->preTbl).push_back( 7);
		return ptr;
}
};
class Regio_v :public Reg {
public:
	Regio_v(int id, int dtype, int nxtNum, int pre_num)
		:Reg(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<1> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Regio_v (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<1> ();
		(*ptr->preTbl).push_back( 8);
		return ptr;
}
};
class Regx :public Reg {
public:
	Regx(int id, int dtype, int nxtNum, int pre_num)
		:Reg(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Regx (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		(*ptr->preTbl).push_back( 9);
		return ptr;
}
};
class Regy :public Reg {
public:
	Regy(int id, int dtype, int nxtNum, int pre_num)
		:Reg(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Regy (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		(*ptr->preTbl).push_back( 10);
		return ptr;
}
};
class Node7 :public Comb {
public:
	Node7(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	void Update() { if (status==Comb::setFlag) return ; status += 1;
		*(UInt<32> *)this->Data =  *(UInt<32>*)((*Node::AllSet)[5]->Data);
		return ;
}
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Node7 (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		return ptr;
}
};

class Node8 :public Comb {
public:
	Node8(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<1> *)this->Data; }
	void Update() { if (status==Comb::setFlag) return ; status += 1;
		*(UInt<1> *)this->Data =  (*(UInt<32>*)((*Node::AllSet)[6]->Data) == UInt<1>(0x0));
		return ;
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
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	void Update() { if (status==Comb::setFlag) return ; status += 1;
		*(UInt<32> *)this->Data =  (*(UInt<1>*)((*Node::AllSet)[2]->Data) ? *(UInt<32>*)((*Node::AllSet)[0]->Data) : ((*(UInt<32>*)((*Node::AllSet)[5]->Data) > *(UInt<32>*)((*Node::AllSet)[6]->Data)) ? ((((*(UInt<32>*)((*Node::AllSet)[5]->Data) - *(UInt<32>*)((*Node::AllSet)[6]->Data)) + *(UInt<32>*)((*Node::AllSet)[6]->Data)) - *(UInt<32>*)((*Node::AllSet)[6]->Data)).tail<1>()) : *(UInt<32>*)((*Node::AllSet)[5]->Data))).bits<31,0>();
		return ;
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
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	void Update() { if (status==Comb::setFlag) return ; status += 1;
		*(UInt<32> *)this->Data =  (*(UInt<1>*)((*Node::AllSet)[2]->Data) ? *(UInt<32>*)((*Node::AllSet)[1]->Data) : (((*(UInt<32>*)((*Node::AllSet)[5]->Data) > *(UInt<32>*)((*Node::AllSet)[6]->Data)) == UInt<1>(0x0)) ? ((*(UInt<32>*)((*Node::AllSet)[6]->Data) - *(UInt<32>*)((*Node::AllSet)[5]->Data)).tail<1>()) : *(UInt<32>*)((*Node::AllSet)[6]->Data)));
		return ;
}
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {		Node * ptr = new Node10 (id,dtype,nxtNum,pre_num);
		ptr->Data = new UInt<32> ();
		return ptr;
}
};

namespace Generate {
	void GenerateNode() {
		(*Node::AllSet).push_back(Regio_a::Create(0,sizeof(UInt<32>),0,2));
		(*Node::RegSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Regio_b::Create(1,sizeof(UInt<32>),0,2));
		(*Node::RegSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Regio_e::Create(2,sizeof(UInt<1>),0,2));
		(*Node::RegSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Regio_z::Create(3,sizeof(UInt<32>),0,2));
		(*Node::RegSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Regio_v::Create(4,sizeof(UInt<1>),0,2));
		(*Node::RegSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Regx::Create(5,sizeof(UInt<32>),0,2));
		(*Node::RegSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Regy::Create(6,sizeof(UInt<32>),0,2));
		(*Node::RegSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Node7::Create(7,sizeof(UInt<32>),0,2));
		(*Node::CombSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Node8::Create(8,sizeof(UInt<1>),0,2));
		(*Node::CombSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Node9::Create(9,sizeof(UInt<32>),0,2));
		(*Node::CombSet).push_back((*Node::AllSet).back());
		(*Node::AllSet).push_back(Node10::Create(10,sizeof(UInt<32>),0,2));
		(*Node::CombSet).push_back((*Node::AllSet).back());
	}
}