#include "pch.h"
#include "Node.h"
#include "List.h"

using namespace std;

struct Simulator {
	typedef pair<int, long long >P;

	vector<P>SetS;
	queue<int>TransQueue;
	void Init(int N) {
		Node::AllSet  = new vector<Node*>;
		Node::CombSet = new vector<Node*>;
		Node::RegSet  = new vector<Node*>;

		Generate::GenerateNode();
	}

# if 1
	void Out() {
		auto& T = (*Node::AllSet);
		for (auto &i:T) {
			if (i->sign) {
				cout << *i->getSInt() << endl;
			}
			else {
				cout << *i->getSInt() << endl;
			}
		}
		putchar(10);
	}

	void RegSet() {
		for (int i = 0; i < SetS.size(); i++) {
			auto& t = SetS[i];
			(*Node::AllSet)[t.first]->SetData(t.second);
		}
	}
	
	void CombTrans() {
		auto& T = (*Node::CombSet);
#if DEBUG
		printf("\nDebug ----\n");
#endif
		for (int i = 0; i < T.size();i++) {
			T[i]->Update();
#if DEBUG
			printf("Comb %d=%u\n",i,T[i]->getUIntData());
			Out();
			//putchar(10);
#endif // DEBUG

		}
		
	}
	void RegTrans() {
		auto& T = (*Node::RegSet);
		for (auto& i :T) {
			i->Update();
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
		/*
		for (int i = 0; i < Node::CombSet->size(); i++) {
				(*Node::CombSet)[i]->GetData();
		}
		*/
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
		Out();
		CombTrans();
		RegTrans();
		putchar(10);
	}

	void run(int N, vector<pair<int, vector<pair<int, int>>>>& Input) { 

		Simulator::Init(N);
		vector<P>input;

		for (int i = 0; i < N; i++) {

		}
		putchar(10);
		int j = 0;
		
		for (int i = 0; i < 10; i++) {
			input.clear();
			if (j < Input.size() && Input[j].first == i) {
				for (auto& t : Input[j].second) {
					input.push_back(make_pair(t.first, t.second));
				}
				j++;
			}
			Simulator::MoveCycle(input);
		}
		Simulator::RegSet();

	}
};

# if 0
vector<pair<int,vector<pair<int,int>>>>Input;
int main() {
	vector<pair<int, int>>t1;
	t1.push_back({ 0,21 }); // 给io_a赋值
	t1.push_back({ 1,9 }); // 给io_b赋值
	t1.push_back({ 2,1 }); // 给io_e赋值

	vector<pair<int, int>>t2;
	t2.push_back({ 0,21 }); // 给io_a赋值
	t2.push_back({ 1,9 }); // 给io_b赋值
	t2.push_back({ 2,0 }); // 给io_e赋值

	Input.push_back(make_pair(0, t1));
	Input.push_back(make_pair(1, t2));
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