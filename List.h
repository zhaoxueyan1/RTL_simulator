#pragma once
#include"Node.h"
#include"UInt.h"
#include"SInt.h"

void Reg111(){
	*(*Node::AllSet)[111]->getUInt() = *(*Node::AllSet)[111]->getUInt() + *(*Node::AllSet)[111]->getUInt();
}

namespace Generate {
	void GenerateNode() {
		(*Node::AllSet).push_back(new Node(111,true,Reg111));
	}
}