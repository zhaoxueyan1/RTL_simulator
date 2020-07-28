#include "VCDFileParser.hpp"
#include"../../../IRDecoder.hpp"
#include <vector>

void print_scope_signals(VCDFile * trace, VCDScope * scope, int instanceID,IRDecoder* p)
{
    for(VCDSignal * signal : scope -> signals) {
        std::cout << trace->get_signal_values(signal -> hash)->size() << "\t"
                    << signal -> reference<<"\t"<<p->InstnceChildMP[instanceID][signal->reference]<<std::endl;
        auto& S = *(trace->get_signal_values(signal -> hash));
        for(auto &i: S){
            std::cout<<i->time<<" ";
            /*
            VCD_SCALAR, //!< Single VCDBit
            VCD_VECTOR, //!< Vector of VCDBit
            VCD_REAL  
            */
            switch (i->value->get_type())
            {
            case VCD_SCALAR:{
                putchar(VCDValue::VCDBit2Char(i->value->get_value_bit()));
                break;
            }
            case VCD_VECTOR: {
                auto tt =  i->value->get_value_vector();
                if(tt && tt->size())for(auto t:*tt){
                    putchar(VCDValue::VCDBit2Char(t));
                }
                break;
            }
            case VCD_REAL:{
                std::cout<<i->value->get_value_real();
                break;
            }
            default:break;
            }
            putchar(10);
        }

    }
}

void traverse_scope(std::string parent, VCDFile * trace, VCDScope * scope, IRDecoder* p)
{
    std::string local_parent = parent;
    if (parent.length()){
        int id = p->InstanceIdMp[{scope->name,local_parent}];
        if (true)
            print_scope_signals(trace, scope, id, p);
    }
    for (auto child : scope->children)
        traverse_scope(scope->name, trace, child, p);
}
/*!
@brief Standalone test function to allow testing of the VCD file parser.
*/

int main (int argc, char** argv)
{

    freopen("./valt_dump_split.out","w",stdout);
    std::string infile = "../../../Data/valt_dump_split.vcd";
    std::string graphInfile = "../../../Data/NoopOutput.in";
    VCDFileParser parser;        
    IRDecoder* p = new IRDecoder();
    p->InitFile(graphInfile);
    VCDFile * trace = parser.parse_file(infile);

    if (trace) {
        if (true) {
            std::cout << "Version:       " << trace -> version << std::endl;
            std::cout << "Comment:       " << trace -> comment << std::endl;
            std::cout << "Date:          " << trace -> date << std::endl;
            std::cout << "Signal count:  " << trace -> get_signals() -> size() <<std::endl;
            std::cout << "Times Recorded:" << trace -> get_timestamps() -> size() << std::endl;
        }    
        // Print out every signal in every scope.
        traverse_scope(std::string(""), trace, trace->root_scope,p);
        
        delete trace;
        return 0;
    } else {
        std::cout << "Parse Failed." << std::endl;
        return 1;
    }
}
