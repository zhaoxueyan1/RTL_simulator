#include"pch.h"
#include"function.h"
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

	IRDecoder(int level=0) :level(level){}
	int level;
	std::vector<int>RegSet;
	std::vector<int>Graph[maxn];
	std::vector<int>reverseGraph[maxn];
	std::vector<int>instanceGraph[300];
	
	// (name,father's name) -> id
	std::map<std::pair<std::string,std::string>,int>InstanceIdMp;
	
	// (id,signal name) -> signal id
	std::map<std::string,int>InstnceChildMP[300];

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
		int typeID;
		std::string expr;
		int id;
		int cppWidth;
	}NodeSet[maxn];

	int getTypeID(std::string type) {
		if (type == "wire") return FuncType::Wire;
		else if (type == "Node") return FuncType::Node;
		else if (type == "Mux") return FuncType::Mux;
		else if (type == "UIntLiteral") return FuncType::UIntLiteral;
		else if (type == "Eq") return FuncType::Eq;
		else if (type == "Port") return FuncType::Port;
		else if (type == "Bits") return FuncType::Bits;
		else if (type == "And") return FuncType::And;
		else if (type == "Reg") return FuncType::Reg;
		else if (type == "Or") return FuncType::Or;
		else if (type == "Cat") return FuncType::Cat;
		else if (type == "ValidIf") return FuncType::ValidIf;
		else if (type == "Add") return FuncType::Add;
		else if (type == "Tail") return FuncType::Tail;
		else if (type == "AsUInt") return FuncType::AsUInt;
		else if (type == "Not") return FuncType::Not;
		else if (type == "Memen") return FuncType::Memen;
		else if (type == "Memdata") return FuncType::Memdata;
		else if (type == "Memclk") return FuncType::Memclk;
		else if (type == "Memaddr") return FuncType::Memaddr;
		else if (type == "Neq") return FuncType::Neq;
		else if (type == "Pad") return FuncType::Pad;
		else if (type == "Memmask") return FuncType::Memmask;
		else if (type == "Lt") return FuncType::Lt;
		else if (type == "Geq") return FuncType::Geq;
		else if (type == "Xor") return FuncType::Xor;
		else if (type == "Dshr") return FuncType::Dshr;
		else if (type == "Dshl") return FuncType::Dshl;
		else if (type == "Sub") return FuncType::Sub;
		else if (type == "Gt") return FuncType::Gt;
		else if (type == "Shl") return FuncType::Shl;
		else if (type == "Shr") return FuncType::Shr;
		else if (type == "AsSInt") return FuncType::AsSInt;
		else if (type == "Mul") return FuncType::Mul;
		else throw "Error";
	}
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

	void ConstructInstanceGraph() {
		InstanceIdMp.insert({std::make_pair(InstanceSet[0].name,"Top"),0});
		for(int i=0;i<InstanceNum;i++) {
			auto & t = instanceGraph[i];
			for(auto z:t){
				InstanceIdMp.insert({std::make_pair(InstanceSet[z].name,InstanceSet[i].name),z});
			}
		}
	}
	/*
	level 0: include Reg Wire Node
	level 1: include Reg Wire
	level 2: include Reg
	*/
	bool getFlag(int typeID) {
		switch (level)
		{
		case 0:return typeID == FuncType::Node || typeID == FuncType::Wire || typeID == FuncType::Reg; break;
		case 1:return typeID == FuncType::Wire || typeID == FuncType::Reg; break;
		case 2:return typeID == FuncType::Reg; break;
		default:
			break;
		}
		throw "Error level";
	}
	std::string PreTrans(int s,int fa) {
		auto& tnode = NodeSet[s];
		std::string res = tnode.type;
		if (tnode.type == "UIntLiteral") {
			res = tnode.expr;
			//assert("");
		}
		auto& p = Graph[s];
		bool flag = getFlag(tnode.typeID);
		if (flag)
		{
			if (fa == -1) {
				res=tnode.name+"=";
			}
			else {
				return tnode.name;
			}
		}
		switch (p.size())
		{
		case 1: {
			switch (tnode.typeID)
			{
			default:
				break;
			}
			return res+ "(" +PreTrans(p[0], s)+")";
		}
		case 2: {
			switch (tnode.typeID)
			{
			default:
				break;
			}
			return res+ "(" + PreTrans(p[0], s)+","+ PreTrans(p[1], s) + ")";
		}
		case 3: {
			switch (tnode.typeID)
			{
			default:
				break;
			}
			return res + "(" + PreTrans(p[0], s) + "," + PreTrans(p[1], s) + "," + PreTrans(p[2], s) + ")";
		}
		default:
			break;
		}
		return res;
	}

	void transform() {


	}

	void Generate(std::string path) {

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
		std::ofstream ofile;
		ofile.open(path);

		for (int i = 0; i < RegSet.size(); i++) {
			ofile <<PreTrans(RegSet[i],-1)<<std::endl;
		}
		ofile.close();
	}


	void InitFile(std::string path) {
		std::ifstream  afile;
		afile.open(path);
		int id, wid,insID;
		std::string name, cppType, dir, type, ext;
		std::string insName, mouName;
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
				insID>>
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
			t.typeID = getTypeID(type);
			IRDecoder::NodeSet[id] = t;
			InstnceChildMP[insID].insert({name,id});
			bool flag = getFlag(t.typeID);
			if (flag) {
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
		ConstructInstanceGraph();
		afile.close();
	}
};

#endif