#ifndef VCDFILE_H_
#define VCDFILE_H_

#include <string>
#include<fstream>

namespace vcd
{

class VcdFileReader
{
public:
    VcdFileReader(std::string fileName);
    ~VcdFileReader();

    /**
     * Read char from the file.
     * Throw an exception if eof is reached.
     */
    char getChar();

    

    /**
     * return next word from the stream (remove possible spaces at the beginning)
     */
    std::string getNextWord();

    /**
     * Return a word from the stream.
     * This is faster impl of getNextWord.
     * call this function only when you are sure 
     * there are not spaces at the beginning of the word.
     */
    std::string getWord();
    
    /**
     * read a word from the file, convert it to decimal number and return it
     * if not convertable throw an VcdParserException exception. 
     */
    int getDecimalNumber();   

    /**
     * trim spaces and return first text chararcter
     */
    char trimSpaces();

    /**
    * read file till $end token 
    */
    void readBufferTillEndToken();

private:
    std::ifstream _vcdFile;    
    bool _eofReached;
};

}
#endif //VCDFILE_H_
