#include"pch.h"

using namespace std;

/*


*/

struct Node {
	string name;
	string cppType;
	string dir;
	string type;
	string expr;

	virtual void generate() = 0;
	int id;
	bool operator<(const Node & b)const {
		return id < b.id;
	}
};

struct Comb: public Node{
	
	void generate() {
		printf("#pragma once\n#include\"Comb.h\"\n#include\"Uint.h\"\n");
		printf("class Add :public Comb {\npublic:\n");
		printf("%s(int id, int dtype, int nxtNum, int pre_num)\n",name.c_str());
		printf("\t\t:Comb(id, dtype, pre_num, nxtNum) {}\n");
		printf("\tvoid* GetData() { if (status==Comb::setFlag) return Data; status += 1;\n");
		printf("\t\t*(%s *)this->Data = %s\n",this->cppType.c_str(),this->expr.c_str());
		printf("\t\treturn Data\n}\n");
		
		printf("\tstatic Node * Create(int id, int dtype, int nxtNum, int pre_num) {");
		printf("\t\tNode * ptr = new %s(id,dtype,nxtNum,pre_num);\n",this->name.c_str());
		printf("\t\tptr->Data = new %s();\n",this->cppType);
		printf("\t\treturn ptr;\n}\n};\n\n");
	}
	
};

struct Reg:public Node {
	
	void generate() {

	}
};

vector<Node*>G;

void Generate() {
	for (int i = 0; i < G.size(); i++) {
		G[i]->generate();
	}
	
}
int main(int argc,char * argv[]) {
	int n;
	cin >> n;
	string id, name, cppType, dir, type, expr;
	for (int i = 0; i < n; i++) {
		cin >> id >> type >> cppType >> dir >> name;
		getline(cin, expr);
	}

	return 0;
}