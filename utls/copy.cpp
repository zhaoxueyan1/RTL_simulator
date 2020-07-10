#include<bits/stdc++.h>

using namespace std;
int maxn = 100000;

int main(){
	const char* inputfile = "../data/vlt_dump.vcd" ;
	const char* outputfile = "../data/valt_dump_split.vcd";
	freopen(inputfile,"r",stdin);
	freopen(outputfile,"w",stdout);
	string t;
	for(int i=0;i<maxn;i++){
		getline(cin,t);
		printf("%s\n",t.c_str());
	}
	
	return 0;
} 
