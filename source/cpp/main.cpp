#include "./include/IRDecoder.h"
#include "./include/SInt.h"
#include "./include/UInt.h"
#include "./include/pch.h"

int main() {
#if 0
    std::string path = "./Data/firout/NOOPSimTop.fir.out";
    IRDecoder* p = new IRDecoder(2);
    p->InitFile(path);
    p->Generate("./Data/GraphRestruct/NOOPSimTop.fir.res");
    //" +std::to_string(clock()) "
#endif

#if 0
    int Data[] = {0, 1, 2, 3, 4};
    Simulator::Init(4);
    vector<P> input;
    for (int i = 0; i < 5; i++) {
        input.clear();
        int* a = new int;
        *a = Data[i];
        P t = make_pair(0, a);
        input.push_back(t);
        Simulator::MoveCycle(input);
    }
    Simulator::RegSet();
#endif

    SInt a(4, 3, true);
    SInt b(2, 3, true);
    std::cout << a / b << std::endl;
    return 0;
}
