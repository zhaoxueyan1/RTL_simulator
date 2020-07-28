#include "pch.h"
#include "IRDecoder.hpp"


int main() {
	std::string path = "./Data/firout/NOOPSimTop.fir.out";
	IRDecoder *p = new IRDecoder(2);
	p->InitFile(path);
	p->Generate("./Data/GraphRestruct/NOOPSimTop.fir.res");//" +std::to_string(clock()) "
	//IRDecoder::InitFile(path);
	return 0;
}