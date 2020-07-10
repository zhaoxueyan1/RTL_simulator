#include"pch.h"

#ifndef IRDECODER_H
#define IRDECODER_H

#define Analysis 0
/*
class Node(var name:String,
		   var id:String,
		   var cppType:String,
		   var cppWidth:BigInt,
		   var dir:String,
		   var tpe:String,
		   var expr:String
		  )

*/

struct IRDecoder {

	static const int maxn = 5e5 + 7;
	std::vector<int>RegSet;
	std::vector<int>Graph[maxn];
	std::vector<int>reverseGraph[maxn];
	std::vector<int>instanceGraph[300];

	int InstanceNum;
	int InstanceEdgeNum;
	int NodeNum;
	int NodeEdgeNum;

	bool vis[maxn];

	struct Instance {
		int id;
		std::string name;
		std::string mouduleName;
	}InstanceSet[300];

	struct Node {
		std::string name;
		std::string cppType;
		std::string dir;
		std::string type;
		std::string expr;
		int id;
		int cppWidth;
	}NodeSet[maxn];;

#if Analysis

	map<string, int>TypeMP;
	vector<pair<int, string>>TypeName;
	map<int, int>WidMP;
	vector<pair<int, int>>WidRank;

#endif

	struct Comb : public Node {

	};

	struct Reg :public Node {

	};

	
	std::string PreTrans(int s,int fa) {
		auto& tnode = NodeSet[s];
		std::string res = tnode.type+" ";
		auto& p = Graph[s];
		if ((tnode.type=="Node"||tnode.type=="wire"|| tnode.type=="Reg") && fa != -1)
			return res;
		for (int i = 0; i < p.size(); i++) {
			auto &t = p[i];
			if (!vis[t]) {
				res += PreTrans(t,s) + " ";
			}
		}
		return res;
	}

	void Generate() {

#if Analysis

		printf("%d\n", TypeMP.size());
		for (auto &i : TypeMP) {
			TypeName.push_back({ i.second,i.first });
		}
		sort(TypeName.begin(), TypeName.end(), greater<pair<int, string>>());
		for (auto &i : TypeName) {
			printf("%s\t\t:%d\n", i.second.c_str(), i.first);
		}
		printf("\n");
		printf("%d\n", WidMP.size());
		for (auto &i : WidMP) {
			WidRank.push_back({ i.second,i.first });
		}
		sort(WidRank.begin(), WidRank.end(), greater<pair<int, int>>());
		for (auto &i : WidMP) {
			printf("%d\t%d\n", i.first, i.second);
		}

#endif

		/*for (int i = 0; i < RegSet.size(); i++) {
			cout<<PreTrans(RegSet[i],-1)<<endl;
		}*/
	}


	void InitFile(std::string path) {
		std::ifstream  afile;
		afile.open(path);
		int id, wid;
		std::string name, cppType, dir, type, ext;
		std::string insID, insName, mouName;
		afile >> InstanceNum;
		for (int i = 0; i < InstanceNum; i++) {
			afile >> id >> insName >> mouName;
			InstanceSet[id] = {id,insName,mouName};
		}
		afile >> InstanceEdgeNum;
		int u, v;
		for (int i = 0; i < InstanceEdgeNum; i++) {
			afile >> u >> v;
			instanceGraph[v].push_back(u);
		}

		afile >> NodeNum;
		for (int i = 0; i < NodeNum; i++) {
			afile >> id >>
				name >>
				type >>
				cppType >>
				wid >>
				dir;
			IRDecoder::Node t;
			getline(afile,ext);
			t.id = id;
			t.name = name;
			t.type = type;
			t.cppType = cppType;
			t.dir = dir;
			t.expr = ext;
			t.cppWidth = wid;
			IRDecoder::NodeSet[id] = t;
			if (type == "Node" || type == "Reg" || type == "wire") {
				RegSet.push_back(id);
			}
#if Analysis

			TypeMP[type] += 1;
			WidMP[wid] += 1;

#endif // Analysis

		}
		afile >> NodeEdgeNum;
		for (int i = 0; i < NodeEdgeNum; i++) {
			afile >> u >> v;
			Graph[u].push_back(v);
			reverseGraph[v].push_back(u);
		}
	}
};

#if 0

int main(int argc, char * argv[]) {
	int n;
	int id;
	int wid;
	string input_file = "./Data/Output.in";
	string outpu_path = "./Data/Analysis2.out";
	freopen(outpu_path.c_str(), "w", stdout);
	IRDecoder::InitFile(input_file);
	IRDecoder::Generate();
	return 0;
}
#endif

#endif