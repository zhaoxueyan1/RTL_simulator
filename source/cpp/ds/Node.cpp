#include"pch.h"
#include"Node.h"


using namespace std;
//#include"GenerateNode.h"


Node::Node(int id, bool sign, Func UpdateFunc)
        :id(id), sign(sign),Updatefunc(UpdateFunc)
{
}

void Node::SetArg(bool sign, int preNum, int nxtNum) {
    this->sign = sign;

}

vector<Node*>* Node::AllSet = nullptr;
vector<Node*>* Node::CombSet = nullptr;
vector<Node*>* Node::RegSet = nullptr;
