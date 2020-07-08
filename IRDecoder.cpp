#include<iostream>
#include<vector>
#include<algorithm>
#include"pch.h"
#define Analysis 0

using namespace std;

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
enum Type
{
	Node	   ,//
	Mux		   ,//
	UIntLiteral,//
	Eq		   ,//
	Port	   ,//
	Bits	   ,//
	And		   ,//
	Reg		   ,//
	Or		   ,//
	Cat		   ,//
	ValidIf	   ,//
	Add		   ,//
	Tail	   ,//
	AsUInt	   ,//
	Not		   ,//
	Memen	   ,//
	Memdata	   ,//
	Memclk	   ,//
	Memaddr	   ,//
	Neq		   ,//
	Pad		   ,//
	Memmask	   ,//
	Lt		   ,//
	Geq		   ,//
	Xor		   ,//
	Dshr	   ,//
	Dshl	   ,//
	Sub		   ,//
	Gt		   ,//
	Shl		   ,//
	Shr		   ,//
	AsSInt	   ,//
	Mul		   ,//
	Leq		   ,//
};
namespace IRDecoder {
	const int maxn = 5e5 + 7;
	vector<int>RegSet;
	vector<int>Graph[maxn];
	
	struct Node {
		string name;
		string cppType;
		string dir;
		string type;
		string expr;
		int id;
		int cppWidth;
	}NodeSet[maxn];;

	struct Comb : public Node {

	};

	struct Reg :public Node {

	};
	bool vis[maxn];
	string PreTrans(int s,int fa) {
		auto& tnode = NodeSet[s];
		string res = tnode.type+" ";
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
		for (int i = 0; i < RegSet.size(); i++) {
			cout<<PreTrans(RegSet[i],-1)<<endl;
		}
	}


}
using namespace IRDecoder;

#if Analysis

map<string, int>TypeMP;
vector<pair<int, string>>TypeName;
map<int, int>WidMP;
vector<pair<int, int>>WidRank;
#endif

#if 1

int main(int argc, char * argv[]) {
	int n;
	int id;
	int wid;
	string input_file = "./Data/Output.in";
	string outpu_path = "./Data/Analysis2.out";
	string name, cppType, dir, type, ext;
	freopen(input_file.c_str(), "r", stdin);
	freopen(outpu_path.c_str(), "w", stdout);

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> id >>
			name >>
			type >>
			cppType >>
			wid>>
			dir;
		IRDecoder::Node t;
		getline(cin, ext);
		t.id = id;
		t.name = name;
		t.type = type;
		t.cppType = cppType;
		t.dir = dir;
		t.expr = ext;
		t.cppWidth = wid;
		IRDecoder::NodeSet[id] = t;
		if (type=="Node"||type == "Reg" || type == "wire") {
			RegSet.push_back(id);
		}
#if Analysis

		TypeMP[type] += 1;
		WidMP[wid] += 1;

#endif // Analysis

	}
	int m;
	cin >> m;
	int u, v;
	for (int i = 0; i < m; i++) {
		scanf("%d %d", &u, &v);
		Graph[u].push_back(v);
	}
	
#if Analysis

	printf("%d\n", TypeMP.size());
	for (auto &i : TypeMP) {
		TypeName.push_back({i.second,i.first});
	}
	sort(TypeName.begin(), TypeName.end(),greater<pair<int,string>>());
	for (auto &i : TypeName) {
		printf("%s\t\t:%d\n",  i.second.c_str(), i.first );
	}
	printf("\n");
	printf("%d\n", WidMP.size());
	for (auto &i : WidMP) {
		WidRank.push_back({i.second,i.first});
	}
	sort(WidRank.begin(), WidRank.end(), greater<pair<int, int>>());
	for (auto &i : WidMP) {
		printf("%d\t%d\n", i.first, i.second);
	}

#endif

	IRDecoder::Generate();
	return 0;
}
#endif