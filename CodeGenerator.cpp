#include<iostream>
#include<vector>
#include<algorithm>
#include"pch.h"

using namespace std;

/*


*/
namespace CodeGenerator {


	struct Node {
		string name;
		string cppType;
		string dir;
		string type;
		string expr;

		virtual void generate() = 0;
		virtual void create() = 0;
		int id;
	};

	struct Comb : public Node {

		void generate() {
			printf("class %s :public Comb {\npublic:\n", name.c_str());
			printf("\t%s(int id, int dtype, int nxtNum, int pre_num)\n", name.c_str());
			printf("\t\t:Comb(id, dtype, pre_num, nxtNum) {}\n");
			printf("\tvoid* GetData() { if (status==Comb::setFlag) return Data; status += 1;\n");
			printf("\t\t*(%s *)this->Data = %s;\n", this->cppType.c_str(), this->expr.c_str());
			printf("\t\treturn Data;\n}\n");

			printf("\tstatic Node * Create(int id, int dtype, int nxtNum, int pre_num) {");
			printf("\t\tNode * ptr = new %s (id,dtype,nxtNum,pre_num);\n", this->name.c_str());
			printf("\t\tptr->Data = new %s ();\n", this->cppType.c_str());
			printf("\t\treturn ptr;\n}\n};\n\n");
		}
		void create() {
			printf("\t\t(*Node::AllSet).push_back(%s::Create(%d,sizeof(%s),0,2));\n", name.c_str(), id, cppType.c_str());
		}
	};

	struct Reg :public Node {

		void generate() {

		}

		void create() {
			printf("\t\tp->push_back(%s);\n", this->expr.c_str());
			printf("\t\t(*Node::AllSet).push_back(Reg::Create(%d,sizeof(%s),0,2,p));\n", id, cppType.c_str());
		}
	};

	vector<pair<int, Node*>>G;

	void Generate() {

		printf("#pragma once\n#include\"Comb.h\"\n#include\"Uint.h\"\n#include\"Reg.h\"\n");
		for (int i = 0; i < G.size(); i++) {
			(G[i].second)->generate();
		}
		printf("namespace Generate {\n\tvoid GenerateNode() {\n");
		printf("\t\tvector<int> *p = new vector<int>;\n");
		for (int i = 0; i < G.size(); i++) {
			if (i != 0)
				printf("\t\tp->clear();\n");
			(G[i].second)->create();
		}
		printf("\t}\n}");
	}
}
#if 0
int main(int argc,char * argv[]) {
	int n;
	
	int id;
	string input_file = "./Input/1.in";
	string outpu_path = "./List.h";
	string name, cppType, dir, type, expr;
	freopen(input_file.c_str(),"r",stdin);
	freopen(outpu_path.c_str(),"w",stdout);
	
	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> id >> type >> cppType >> dir >> name;
		CodeGenerator::Node *t;
		if (type == "Comb") {
			t = new CodeGenerator::Comb();
		}
		else
			t = new CodeGenerator::Reg();
		getline(cin, expr);
		t->id		= id		;
		t->type 	= type 		;
		t->name		= name		;
		t->cppType 	= cppType 	;
		t->dir		= dir		;
		t->expr		= expr		;
		//printf("%s\n", expr.c_str());
		CodeGenerator::G.push_back({ id,t });
	}

	sort(CodeGenerator::G.begin(), CodeGenerator::G.end());
	CodeGenerator::Generate();
	return 0;
}
#endif