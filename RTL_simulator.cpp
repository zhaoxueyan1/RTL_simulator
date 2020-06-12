#include "pch.h"
#include "Node.h"
#include "Reg.h"
#include "ReOrder.h"
#include "Comb.h"
#include "GenerateNode.h"

using namespace std;
typedef pair<int, void*>P;

namespace Simulator {


	set<P>SetS;
	queue<int>TransQueue;
	void Init(int N) {
		Node::AllSet = new vector<Node*>;
		Node::CombSet = new vector<Node*>;
		Node::RegSet = new vector<Node*>;

		Generate::GenerateNode();
		for (auto i : (*Node::AllSet)) {
			//cout << typeid(*i).name()<< endl;
			if (typeid(*i) == typeid(Reg))
			{
				Node::RegSet->push_back(i);
			}
			else {
				Node::CombSet->push_back(i);
			}
		}
		//Node::Gptr = (Node *)malloc(N * sizeof(Node*));
		/*for (auto &i : *Node::MP) {
			Node::Gptr[i.first] = *(i.second);
		}*/
		//Node::MP.clear();
	}

	//void RegTrans() {

	//	
	//	putchar(10);
	//}
	void RegSet() {
		while (!TransQueue.empty())
			TransQueue.pop();
		auto j = SetS.begin();
		for (int i = 0; i < Node::RegSet->size(); i++) {
			Reg * t = dynamic_cast<Reg*>((*Node::RegSet)[i]);
			if (j!=SetS.end()&&i == j->first) {
				((Node *)t)->setData(j->second);
				j++;
			}
			/*t->GetData();
			t->setData();*/
			switch (t->dType)
			{
			case 1: printf("%u ", *((UINT_8  *)t->GetData())); break;
			case 2: printf("%u ", *((UINT_16 *)t->GetData())); break;
			case 4: printf("%u ", *((UINT_32 *)t->GetData())); break;
			case 8: printf("%llu ", *((UINT_64 *)t->GetData())); break;
			default:
				break;
			}
			int k = 0;
			for(int k=0;k<t->nxtTbl->size();k++){
			
				if ((*t->nxtTbl)[k] >=Node::RegSet->size()) {
					TransQueue.push((*t->nxtTbl)[k]);
				}
			}

		}
		for (int i = 0; i < Node::CombSet->size(); i++) {
			Node * t = (*Node::CombSet)[i];
			switch (t->dType)
			{
			case 1: printf("%u ", *((UINT_8  *)  t->getData())); break;
			case 2: printf("%u ", *((UINT_16 *)  t->getData())); break;
			case 4: printf("%u ", *((UINT_32 *)  t->getData())); break;
			case 8: printf("%llu ", *((UINT_64 *)t->getData())); break;
			default:
				break;
			}
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
			t->GetData();
			for (int i = 0; i < t->nxtTbl->size(); i++) {
				int tt = (*t->nxtTbl)[i];
				if ( tt>= Node::RegSet->size()&&!vis[tt]) {
					TransQueue.push(tt);
				}
				else {
					SetS.insert(make_pair((*t->nxtTbl)[i],t->GetData() ));
				}
			}
		}
	}

	void MoveCycle(vector<P>& t) {
		for (auto i : t) {
			SetS.insert(i);
		}
		RegSet();
		SetS.clear(); 
		Comb::setFlag += 1;
		CombTrans();
		//RegTrans();
	}
}
# if 1
int Data[] = { 0,1,2,3,4 };
int main() {
	Simulator::Init(4);
	vector<P>input;
	for (int i = 0; i < 5; i++) {
		input.clear();
		int* a = new int;
		*a = Data[i];
		P t = make_pair(0, a);
		input.push_back(t);
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