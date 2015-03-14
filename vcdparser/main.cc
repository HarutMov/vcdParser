#include "VcdParser.h"
#include "VcdParserException.h"

#include <iostream>



int main ()
{
    try {
        vcd::VcdParser vcd("test.vcd");
        vcd.parse();
    }
    catch (VcdParserException* ptr)
    {
        std::cout << ptr->getMessage() << std::endl;
    }
    catch (...)
    {
        std::cout << "Exception occured which doesn't have type of VcdParserException."
            << std::endl;
    }
}
