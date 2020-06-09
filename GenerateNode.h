#pragma once
#include"pch.h"
#include"Comb.h"
#include"Reg.h"
#include"Add.h"

namespace Generate {
	//Gptr[cnt++] = t;
	//MP["Add"] = t 
	/*
	*/

	void GenerateNode() {
		(*Node::AllSet).push_back(Add::GetInstance());
	}
	
}