#include "pch.h"
#include "Node.h"
#include "Reg.h"
#include "ReOrder.h"
#include "Comb.h"
#include "GenerateNode.h"

using namespace std;
vector<pair<int, void*>>SetS;
queue<int>TransQueue;
static void Init(int N) {
	Node::AllSet = new vector<Node*>;
	Node::CombSet= new vector<Node*>;
	Node::RegSet = new vector<Node*>;
	
	Generate::GenerateNode();
	//Node::Gptr = (Node *)malloc(N * sizeof(Node*));
	/*for (auto &i : *Node::MP) {
		Node::Gptr[i.first] = *(i.second);
	}*/
	//Node::MP.clear();
}

void RegTrans() {
	for (int i = 0; i < Node::RegSet->size(); i++) {

	}
}
void RegSet() {
	while (!TransQueue.empty())
		TransQueue.pop();
	for (auto &i:SetS) {
		(*Node::AllSet)[i.first]->setData(i.second);
		TransQueue.push(i.first);
	}
}
void CombTrans() {
	/*for (int i = 0; i < Node::CombSet->size(); i++) {
		 (*Node::CombSet)[i]->GetData();
	}*/
	while (TransQueue.empty()) {
		int u = TransQueue.front(); TransQueue.pop();
		Node * t = (*Node::AllSet)[u];
		t->GetData();
		for (int i = 0; i < t->nxtTbl->size(); i++) {
			if ((*t->nxtTbl)[i] < Node::CombSet->size()) {
				TransQueue.push((*t->nxtTbl)[i]);
			}
		}
	}
}

int main() {

	while (true) {
		Comb::setFlag += 1;
		RegSet();
		CombTrans();
		RegTrans();
	
	}
	return 0;
}