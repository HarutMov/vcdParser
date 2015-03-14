#ifndef VCD_PARSER_H_
#define VCD_PARSER_H_

#include "VcdTokens.h"
#include "VcdFileReader.h"
#include "VcdTimescale.h"
#include "VcdVar.h"

#include <map>

namespace vcd
{

class VcdParser
{
public:
    VcdParser(std::string filename);

    ~VcdParser();

    /**
    * parse vcd file
    */
    void parse();

private:
    VcdTokens::Token getToken();
    
    VcdTokens::VarType getVarType();
    

    int getVarSize();
    
    std::string getIdentifierCode();
    
    std::string getReference();

    void handleEndDefinitions();

    void handleComment();

    void handleDate();

    void handleScope();

    void handleTimescale();
    
    void handleUpscope();

    void handleVersion();

    void handleVar();

    void parseHeader();

    void handleSimulationDumpCommands();

    int getSimulationTime();
    

    void updateScalarValue(std::string newVal);

    void updateRealNumberValue();

    void updateVectorValue();

    void parseBody();

private:
    VcdFileReader* _vcdFileReader;
    VcdTimescale _timescale;
    std::multimap<std::string, VcdVar> _varMap;
    
    std::string _dateString;
    std::string _versionString;
    int _currentTime;
}; 

}

#endif //VCD_PARSER_H_
