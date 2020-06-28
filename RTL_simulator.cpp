#include "pch.h"
#include "Node.h"
#include "Reg.h"
#include "ReOrder.h"
#include "Comb.h"
#include "List.h"

using namespace std;
typedef pair<int, void* >P;

namespace Simulator {

	vector<P>SetS;
	queue<int>TransQueue;
	void Init(int N) {
		Node::AllSet = new vector<Node*>;
		Node::CombSet = new vector<Node*>;
		Node::RegSet = new vector<Node*>;

		Generate::GenerateNode();
		/*
		for (auto i : (*Node::AllSet)) {
			//cout << typeid(*i).name()<< endl;
			if (typeid(*i) == typeid(Reg)) {
				Node::RegSet->push_back(i);
			}
			else {
				Node::CombSet->push_back(i);
			}
		}
		*/
	}
# if 1
	void RegSet() {
		for (int i = 0; i < SetS.size(); i++) {
			auto& t = SetS[i];
			(*Node::AllSet)[t.first]->SetData(t.second);
		}

	}
	
	void CombTrans() {
		auto& T = (*Node::CombSet);
		for (int i = 0; i < T.size();i++) {
			T[i]->Update();
			printf("%u\t",T[i]->getUIntData());
		}
	}

	void RegTrans() {
		auto& T = (*Node::RegSet);
		for (int i = 0; i < T.size(); i++) {
			Reg* t = dynamic_cast< Reg*>(T[i]);
			t->Update();
			printf("%u\t", t->getUIntData());
		}
	}
# endif

# if 0
	void RegSet() {
		while (!TransQueue.empty())
			TransQueue.pop();
		auto j = SetS.begin();
		for (int i = 0; i < Node::RegSet->size(); i++) {
			Reg * t = dynamic_cast<Reg*>((*Node::RegSet)[i]);
			if (j!=SetS.end()&&i == j->first) {
				int* e = (int*)(*(Node *)t).Data;
				printf("%s", typeid(e).name);
				j++;
			}
			/*t->GetData();
			t->setData();*/
			printf("%u ", *((UINT_64  *)t->getData()));
			int k = 0;
			for(int k=0;k<t->nxtTbl->size();k++){
			
				if ((*t->nxtTbl)[k] >=Node::RegSet->size()) {
					TransQueue.push((*t->nxtTbl)[k]);
				}
			}

		}
		for (int i = 0; i < Node::CombSet->size(); i++) {
			Node * t = (*Node::CombSet)[i];
			printf("%u ", *((UINT_64  *)t->getData()));
			
		}
		putchar(10);
	}

	void CombTrans() {
		/*for (int i = 0; i < Node::CombSet->size(); i++) {
				(*Node::CombSet)[i]->GetData();
		}*/
		bool vis[10];
		memset(vis, 0, sizeof(vis));
		while (!TransQueue.empty()) {
			int u = TransQueue.front(); TransQueue.pop();
			if (vis[u])
				continue;
			vis[u] = true;
			Node * t = (*Node::AllSet)[u];
			t->getUIntData();
			for (int i = 0; i < t->nxtTbl->size(); i++) {
				int tt = (*t->nxtTbl)[i];
				if ( tt>= Node::RegSet->size()&&!vis[tt]) {
					TransQueue.push(tt);
				}
				else {
					SetS.push_back(make_pair((*t->nxtTbl)[i],UINT_64(t->getUIntData())	));
				}
			}
		}
	}
#endif
	void MoveCycle(vector<P>& t) {
		for (auto i : t) {
			SetS.push_back(i);
		}
		RegSet();
		SetS.clear(); 
		Comb::setFlag += 1;
		CombTrans();
		RegTrans();
		putchar(10);
	}


}

# if 1
int Data[] = { 0,1,2,3,4 };
int main() {
	Simulator::Init(4);
	vector<P>input;

	printf("io_a\tio_b\tio_e\tio_z\tio_v\tx\ty\t");
	putchar(10);
	for (int i = 0; i < 5; i++) {
		input.clear();	
		input.push_back(make_pair(0, new UInt<32>(Data[i])));
		Simulator::MoveCycle(input);
	}
	Simulator::RegSet();
	return 0;
}
# endif

# if 0
int main() {
	P t;
	vector<P>t2;
	return 0;
}
# endif