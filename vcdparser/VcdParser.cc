#include "VcdParser.h"
#include "VcdFileReader.h"
#include "VcdTokens.h"
#include "VcdParserException.h"


vcd::VcdParser::VcdParser(std::string filename)
{
    _vcdFileReader = new VcdFileReader(filename);
}

vcd::VcdTokens::Token vcd::VcdParser::getToken()
{
    return vcd::VcdTokens::stringToToken(_vcdFileReader->getWord());
}

vcd::VcdTokens::VarType vcd::VcdParser::getVarType()
{
    return vcd::VcdTokens::stringToVarType(_vcdFileReader->getNextWord());
}

int vcd::VcdParser::getVarSize()
{
    return _vcdFileReader->getDecimalNumber();
}

std::string vcd::VcdParser::getIdentifierCode()
{
    return _vcdFileReader->getNextWord();
}

std::string vcd::VcdParser::getReference()
{
    return _vcdFileReader->getNextWord();
}

void vcd::VcdParser::handleEndDefinitions()
{
    char ch = _vcdFileReader->trimSpaces();
    if (ch == '$')
    {
        if (vcd::VcdTokens::END == getToken())
            return;
    }
    throw new VcdParserException("enddefinitions declaration issue.");
}

void vcd::VcdParser::handleComment()
{
    _vcdFileReader->readBufferTillEndToken();
}

void vcd::VcdParser::handleDate()
{
    char ch = _vcdFileReader->trimSpaces();
    while (ch != '$')
    {
        _dateString += ch;
        ch = _vcdFileReader->getChar();
    }

    if (vcd::VcdTokens::END != getToken())
        throw new VcdParserException("date command doesn't have ending keyword");
}

void vcd::VcdParser::handleScope()
{
    _vcdFileReader->readBufferTillEndToken();
}

void vcd::VcdParser::handleTimescale() 
{
    _timescale.time_number = "";
    _timescale.time_unit = "";
    char ch = _vcdFileReader->trimSpaces();
    while (ch >= '0' && ch <= '9')
    {
        _timescale.time_number += ch;
        ch = _vcdFileReader->getChar();
    }
    if (ch == ' ' || ch == '\t' || ch != '\n')
        ch = _vcdFileReader->trimSpaces();
    
    
    _timescale.time_unit += ch;
    ch = _vcdFileReader->getChar();
    if (ch != ' ' || ch != '\t' || ch != '\n')
    {
        _timescale.time_unit += ch;
    }
    
    ch = _vcdFileReader->trimSpaces();
    while (ch != '$')
    {
        ch = _vcdFileReader->getChar();
    }

    if (vcd::VcdTokens::END != getToken())
        throw new VcdParserException("timescale command doesn't have ending keyword");
}

void vcd::VcdParser::handleUpscope()
{
    char ch = _vcdFileReader->trimSpaces();
    if (ch == '$')
    {
        if (vcd::VcdTokens::END == getToken())
            return;
    }
    throw new VcdParserException("upscope command declaration issue.");
}

void vcd::VcdParser::handleVersion()
{
    char ch = _vcdFileReader->trimSpaces();
    while (ch != '$')
    {
        _versionString += ch;
        ch = _vcdFileReader->getChar();
    }
    if (vcd::VcdTokens::END != getToken())
        throw new VcdParserException("version command doesn't have ending keyword");
}

void vcd::VcdParser::handleVar()
{
    vcd::VcdTokens::VarType varType = getVarType();
    if (varType == vcd::VcdTokens::INVALID_VAR_TYPE)
        throw new VcdParserException("Invalid var type.");
    
    int size = getVarSize();
    
    std::string id = getIdentifierCode();
    std::string reference = getReference();
    
    char ch = _vcdFileReader->trimSpaces();
    std::string index = "";
    if (ch == '[')
    {
        index += ch;
        ch = _vcdFileReader->getChar();
        while (ch != ']')
        {
            index += ch;
            ch = _vcdFileReader->getChar();
        }
        index += ch;
        ch = _vcdFileReader->trimSpaces();
    } 
    
    if (ch == '$')
    {
        if (vcd::VcdTokens::END != getToken())
            throw new VcdParserException("Malformed var declaration.");
    }
    
    VcdVar var(varType, size, id, reference, index);
    
    _varMap.insert(std::pair<std::string, VcdVar>(id, var));
}

void vcd::VcdParser::parseHeader()
{
    while (true)
    {
        char ch = _vcdFileReader->trimSpaces();
        if (ch == '$')
        {
            vcd::VcdTokens::Token token = getToken();
            switch (token)
            {
                case vcd::VcdTokens::COMMENT:
                    handleComment();
                    continue;
                case vcd::VcdTokens::DATE:
                    handleDate();
                    continue;
                case vcd::VcdTokens::ENDDEFINITIONS:
                    handleEndDefinitions();
                    // header parsing completed
                    return;
                case vcd::VcdTokens::SCOPE:
                    handleScope();
                    continue;
                case vcd::VcdTokens::TIMESCALE:
                    handleTimescale();
                    continue;
                case vcd::VcdTokens::UPSCOPE:
                    handleUpscope();
                    continue;
                case vcd::VcdTokens::VAR:
                    handleVar();
                    continue;
                case vcd::VcdTokens::VERSION:
                    handleVersion();
                    continue;
                case vcd::VcdTokens::DUMBALL:
                case vcd::VcdTokens::DUMPOFF:
                case vcd::VcdTokens::DUMPON:
                case vcd::VcdTokens::DUMPVARS:
                    throw new VcdParserException("The enddifinition command should be declared before simulation commands.");
                case vcd::VcdTokens::INVALID_TOKEN:
                default:
                    throw new VcdParserException("Invalid Token.");
            }
        }
        else
        {
            throw new VcdParserException("VCD header should start with declaration commands");
        }
    }
}

void vcd::VcdParser::handleSimulationDumpCommands()
{
    while (true)
    {
        char ch = _vcdFileReader->trimSpaces();
        if (ch == '$')
        {
            if (vcd::VcdTokens::END != getToken())
                throw new VcdParserException("Simulation dump command doesn't have ending keyword");
            
            return;
        }
        else if (ch == '0')
        {
            updateScalarValue("0");
        }
        else if (ch == '1')
        {
            updateScalarValue("1");
        }
        else if (ch == 'x' || ch == 'X')
        {
            updateScalarValue("x");
        }
        else if (ch == 'z' || ch == 'Z')
        {
            //do we ever get here?
            updateScalarValue("z");
        }
        else if (ch == 'b' || ch == 'B')
        {
            updateVectorValue();
        }
        else if (ch == 'r' || ch == 'R')
        {
            updateRealNumberValue();
        }
        else
        {
            throw new VcdParserException("Unknown symbol.");
        }
        
    }
}

int vcd::VcdParser::getSimulationTime()
{
    return _vcdFileReader->getDecimalNumber();
}

void vcd::VcdParser::updateScalarValue(std::string newVal)
{
    std::string id = _vcdFileReader->getWord();

    std::pair<std::multimap<std::string, VcdVar>::iterator, std::multimap<std::string, VcdVar>::iterator> ret;
    ret = _varMap.equal_range(id);

    //check if there is at least one element with this id
    if(ret.first == ret.second)
        throw new VcdParserException("No variable with " + id + " identifier.");
    
    for (std::multimap<std::string, VcdVar>::iterator it = ret.first; it != ret.second; ++it)
    {
        (it->second).value = newVal;
    }
}

void vcd::VcdParser::updateRealNumberValue()
{
    std::string newVal = _vcdFileReader->getWord();
    std::string id = _vcdFileReader->getNextWord();
    
    std::pair<std::multimap<std::string, VcdVar>::iterator, std::multimap<std::string, VcdVar>::iterator> ret;
    ret = _varMap.equal_range(id);

    //check if there is at least one element with this id
    if(ret.first == ret.second)
        throw new VcdParserException("No variable with " + id + " identifier.");

    for (std::multimap<std::string, VcdVar>::iterator it = ret.first; it != ret.second; ++it)
    {
        (it->second).value = newVal;
    }
}

void vcd::VcdParser::updateVectorValue()
{
    std::string newVal = _vcdFileReader->getWord();
    std::string id = _vcdFileReader->getNextWord();
    
    std::pair<std::multimap<std::string, VcdVar>::iterator, std::multimap<std::string, VcdVar>::iterator> ret;
    ret = _varMap.equal_range(id);

    //check if there is at least one element with this id
    if(ret.first == ret.second)
        throw new VcdParserException("No variable with " + id + " identifier.");

    for (std::multimap<std::string, VcdVar>::iterator it = ret.first; it != ret.second; ++it)
    {
        (it->second).value = newVal;
    }
}

void vcd::VcdParser::parseBody()
{
    while (true)
    {
        char ch = _vcdFileReader->trimSpaces();
        if (ch == '$')
        {
            vcd::VcdTokens::Token token = getToken();
            switch (token)
            {
                case vcd::VcdTokens::COMMENT:
                case vcd::VcdTokens::DUMBALL:
                case vcd::VcdTokens::DUMPOFF:
                case vcd::VcdTokens::DUMPON:
                case vcd::VcdTokens::DUMPVARS:
                    handleSimulationDumpCommands();
                    continue;
                case vcd::VcdTokens::INVALID_TOKEN:
                    throw new VcdParserException("Invalid Token.");
                default:
                    throw new VcdParserException("Declaration command during the simulation.");
            }
        }
        else if (ch == '#')
        {
            _currentTime = getSimulationTime();
            continue;
        }
        else if (ch == '0')
        {
            updateScalarValue("0");
        }
        else if (ch == '1')
        {
            updateScalarValue("1");
        }
        else if (ch == 'x' || ch == 'X')
        {
            updateScalarValue("x");
        }
        else if (ch == 'z' || ch == 'Z')
        {

            updateScalarValue("z");
        }
        else if (ch == 'b' || ch == 'B')
        {
            updateVectorValue();
        }
        else if (ch == 'r' || ch == 'R')
        {
            updateRealNumberValue();
        }
        else
        {
            throw new VcdParserException("Unknown symbol.");
        }
        
    }
}
    

void vcd::VcdParser::parse()
{
    parseHeader();
    parseBody();
}

vcd::VcdParser::~VcdParser()
{
    delete _vcdFileReader;
}

