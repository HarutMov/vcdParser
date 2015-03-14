#ifndef VCD_PARSER_EXCEPTION_H_
#define VCD_PARSER_EXCEPTION_H_

#include <exception>
#include <string>

class VcdParserException
{
public:
    VcdParserException(std::string message)
        : _myMessage(message)
    {
    }

    std::string getMessage() {return _myMessage;}

private:
    std::string _myMessage;
};

#endif //VCD_PARSER_EXCEPTION_H_
