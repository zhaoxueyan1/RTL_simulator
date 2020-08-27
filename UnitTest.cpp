#include "pch.h"
#include "IRDecoder.hpp"
#include "SInt.h"
#include "UInt.h"

int main() {

#if 0
	std::string path = "./Data/firout/NOOPSimTop.fir.out";
	IRDecoder *p = new IRDecoder(2);
	p->InitFile(path);
	p->Generate("./Data/GraphRestruct/NOOPSimTop.fir.res");//" +std::to_string(clock()) "
#endif

	SInt a(4, 3, true);
	SInt b(2, 3, true);
	std::cout << a / b << std::endl;
	return 0;
}