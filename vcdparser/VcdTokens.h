#ifndef VCD_TOKENS_H_
#define VCD_TOKENS_H_

#include <string>

namespace vcd
{

class VcdTokens
{
public:

enum Token {
    COMMENT, DATE, ENDDEFINITIONS, SCOPE, TIMESCALE, UPSCOPE, VAR, VERSION, END,
    DUMBALL, DUMPOFF, DUMPON, DUMPVARS, INVALID_TOKEN 
};

static Token stringToToken(std::string token)
{
    if (token == "comment") return COMMENT;
    if (token == "date") return DATE;
    if (token == "enddefinitions") return ENDDEFINITIONS;
    if (token == "scope") return SCOPE;
    if (token == "timescale") return TIMESCALE;
    if (token == "upscope") return UPSCOPE;
    if (token == "var") return VAR;
    if (token == "version") return VERSION;
    if (token == "end") return END;
    if (token == "dumpall") return DUMBALL;
    if (token == "dumpoff") return DUMPOFF;
    if (token == "dumpon") return DUMPON;
    if (token == "dumpvars") return DUMPVARS;
    return INVALID_TOKEN;
}

enum VarType {
    EVENT, INTEGER, PARAMETER, REAL, REG, SUPPLY0, SUPPLY1, TIME,
    TRI, TRIAND, TRIOR, TRIGER, TRI0, TRI1, WAND, WIRE, WOR, INVALID_VAR_TYPE
};

static VarType stringToVarType(std::string vartype)
{
    if (vartype == "event") return EVENT;
    if (vartype == "integer") return INTEGER;
    if (vartype == "parameter")    return PARAMETER;
    if (vartype == "real") return REAL;
    if (vartype == "reg") return REG;
    if (vartype == "supply0") return SUPPLY0;
    if (vartype == "supply1") return SUPPLY1;
    if (vartype == "time") return TIME;
    if (vartype == "tri") return TRI;
    if (vartype == "triand") return TRIAND;
    if (vartype == "trior") return TRIOR;
    if (vartype == "triger") return TRIGER;
    if (vartype == "tri0") return TRI0;
    if (vartype == "tri1") return TRI1;
    if (vartype == "wand") return WAND;
    if (vartype == "wire") return WIRE;
    if (vartype == "wor") return WOR;
    return INVALID_VAR_TYPE;
}

};

}

#endif //VCD_TOKENS_H_
