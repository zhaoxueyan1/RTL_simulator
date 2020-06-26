#pragma once
#include"pch.h"
#include"Comb.h"
#include"Reg.h"
#include"Add.h"

namespace Generate {
	void GenerateNode() {
		vector<int> * preTbl = new vector<int>;
		vector<int> * nxtTbl = new vector<int>;
		/*preTbl->push_back();
		preTbl->push_back();*/
		nxtTbl->push_back(2);
		nxtTbl->push_back(3);

		(*Node::AllSet).push_back(Reg::Create(0,4,0,2,preTbl,nxtTbl));
		
		preTbl = new vector<int>;
		nxtTbl = new vector<int>;

		preTbl->push_back(3);
		nxtTbl->push_back(2);
		//preTbl->push_back();
		//nxtTbl->push_back(1);

		(*Node::AllSet).push_back(Reg::Create(1, 4, 1, 1, preTbl, nxtTbl));
	

		preTbl = new vector<int>;
		nxtTbl = new vector<int>;

		preTbl->push_back(0);
		preTbl->push_back(1);
		nxtTbl->push_back(3);
		//nxtTbl->push_back(1);
		(*Node::AllSet).push_back(Add::Create(2, 4, 2, 1));
		
		preTbl = new vector<int>;
		nxtTbl = new vector<int>;

		preTbl->push_back(0);
		preTbl->push_back(2);
		nxtTbl->push_back(1);

		(*Node::AllSet).push_back(Add::Create(3, 4, 2, 1));

	}
	
}