#include "VcdFileReader.h"
#include "VcdParserException.h"

#include <sstream>

vcd::VcdFileReader::VcdFileReader(std::string fileName)
    : _eofReached(false)
{
    _vcdFile.open(fileName.c_str());

    if (!_vcdFile.is_open()) {
        throw new VcdParserException("Error occured while openning vcd file.");
    }
}

vcd::VcdFileReader::~VcdFileReader()
{
    _vcdFile.close();
}

char vcd::VcdFileReader::getChar()
{
    int i = _vcdFile.get();
        
    if (-1 == i)
    {
        if (_eofReached)
            throw new VcdParserException("Done reading.");
        
        _eofReached = true;
    }
    return (char)i;
}

std::string vcd::VcdFileReader::getNextWord()
{
    char ch = trimSpaces();
    std::string str = "";
    str += ch;
    ch = getChar();
    while(ch != ' ' && ch != '\n' && ch != '\t' && !_eofReached) {
        str += ch;
        ch = getChar();
    }
    return str;
}
    

std::string vcd::VcdFileReader::getWord()
{
    std::string str = "";
    char ch = getChar();
    while(ch != ' ' && ch != '\n' && ch != '\t' && !_eofReached) {
        str += ch;
        ch = getChar();
    }
    return str;
}
    
int vcd::VcdFileReader::getDecimalNumber()
{
    int n = 0;
    std::istringstream ss(getWord());
    ss.imbue(std::locale::classic());
    ss >> n;
    if (ss.fail())
        throw new VcdParserException("String to int conversion error.");
    return n;
}


char vcd::VcdFileReader::trimSpaces()
{
    char ch = getChar();
    while(ch == ' ' || ch == '\n' || ch == '\t' || _eofReached /*helps to exit on trimming*/) {
        ch = getChar();
    }
    return ch;
}
    
void vcd::VcdFileReader::readBufferTillEndToken()
{
    std::string str = "";
    while (str != "$end")
    {
        str = getNextWord();
    }
}
