#ifndef __M_LEKSICH_ANALIZER__
#define __M_LEKSICH_ANALIZER__

#include <string>
#include <vector>
#include <memory>

#include "token.h"

class LeksichAnalizer
{
    std::string input;
    size_t position;
    //
    void skipWhiteSpace();
    std::unique_ptr<Token> parseNumber();
    std::unique_ptr<Token> parsePeremOrOneArgOper();
    char getCurrentChar() const;
    void goNextPos();
public:
    LeksichAnalizer(const std::string& inputStr);

    std::vector<std::unique_ptr<Token>> tokenize();
};

#endif

