class Reg1 :public Reg
{
public:
	Reg1(int id, int dtype, int nxtNum, int pre_num = 1) 
		:Reg(id,dtype, nxtNum,pre_num){}
	UINT_64 getUIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	//SINT_64 getSIntData() { return (UINT_64)*(UInt<32> *)this->Data; }
	static Node * Create(int id, int dtype, int nxtNum, int pre_num) { Node * ptr = new Reg1(id, dtype, nxtNum, 1);
		ptr->Data = new UInt<32>(0);
		(*ptr->preTbl).push_back(1);
		return ptr;
	}
};

class Add :public Comb {
public:
	Add(int id, int dtype, int nxtNum, int pre_num)
		:Comb(id, dtype, pre_num, nxtNum) {}
	void Update() { if (status == Comb::setFlag) return ; status += 1;
		*(UInt<32>*)this->Data = *(UInt<32>*)((*Node::AllSet)[0]->Data) + *(UInt<32>*)((*Node::AllSet)[0]->Data);
		return ;
	}

	static Node * Create(int id, int dtype, int nxtNum, int pre_num) {
		Node * ptr = new Add(id, dtype, nxtNum, pre_num);
		ptr->Data = new UInt<32>();
		return ptr;
	}
};

namespace Generate {
	void GenerateNode() {
		(*Node::AllSet).push_back(Reg1::Create(0,sizeof(UInt<32>),0,2));
		(*Node::AllSet).push_back(Reg1::Create(1,sizeof(UInt<32>),0,2));
    }
}