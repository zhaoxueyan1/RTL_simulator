#include"pch.h"


#if 0

#include"RTL_simulator.cpp"

using namespace std;
int Data[] = { 0,1,2,3,4 };
int main() {
	Simulator::Init(4);
	vector<P>input;
	for (int i = 0; i < 5; i++) {
		input.clear();
		int* a = new int;
		*a = Data[i];
		P t = make_pair(0, a);
		input.push_back(t);
		Simulator::MoveCycle(input);
	}
	Simulator::RegSet();
	return 0;
}

#endif