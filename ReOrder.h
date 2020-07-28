#pragma once
#include"pch.h"
using namespace std;

namespace ReOrder {
	const int maxn = 1e6 + 7;
	vector<int>* G;
	map<string, int>* Map;
	/*int cnt;*/
	int* reMap;
	int* inDegree;
	//int* outDegree;
	int N;
	void init(int n) {
		Map = new map<string,int>;
		N = n;
		inDegree = (int*)malloc(n * sizeof(int));
		//outDegree = (int*)malloc(n*sizeof(int));
		reMap = (int*)malloc(n * sizeof(int));
		G = (vector<int>*)malloc(n * sizeof(vector<int>));
	}

	void destroy() {
		if (inDegree)
			free(inDegree);
		/*if (outDegree)
			free(outDegree);*/
		for (int i = 0; i < N; i++) {
			G[i].clear();
		}
		if (G)
			free(G);
		if (reMap)
			free(reMap);
	}
	void add(string s1, string s2) {
		int u = (*Map)[s1];
		int v = (*Map)[s2];
		G[u].push_back(v);
		inDegree[v]++;
		//outDegree[cnt - 1]++;
	}
	
}