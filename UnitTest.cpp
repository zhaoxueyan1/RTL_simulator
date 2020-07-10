#include "pch.h"
#include "IRDecoder.h"

int main() {
	std::string path = "./Data/NoopOutput.in";
	IRDecoder *p = new IRDecoder();
	p->InitFile(path);
	//IRDecoder::InitFile(path);
	return 0;
}