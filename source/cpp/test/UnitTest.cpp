#include "pch.h"
#include "IRDecoder.hpp"
#include "Sint.h"

#if 0
int main() {
    std::string path = "./Data/firout/NOOPSimTop.fir.out";
    IRDecoder *p = new IRDecoder(2);
    p->InitFile(path);
    p->Generate("./Data/GraphRestruct/NOOPSimTop.fir.res");//" +std::to_string(clock()) "
    SInt a(32,0);
    SInt b(32,0);
    SInt c = a + b;
    return 0;
}
#endif
