#ifndef VCD_VAR_H_
#define VCD_VAR_H_

#include "VcdTokens.h"
#include <string>

namespace vcd
{

struct VcdVar
{
    VcdTokens::VarType varType;
    unsigned int size;
    std::string id;
    std::string index; // is not mandator field
    std::string reference;
    std::string value;

    VcdVar(VcdTokens::VarType vt, int s, std::string d, std::string i, std::string ref)
    : varType(vt)
    , size(s)
    , id(d)
    , index(i)
    , reference(ref)
    {
    }
};

}

#endif //VCD_VAR_H_
