#ifndef __M_LEKSICH_ANALIZER__
#define __M_LEKSICH_ANALIZER__

#include <string>
#include <vector>

#include "token.h"

class LeksichAnalizer
{
    std::string input;
    size_t position;
    int line, column;
    //
    void skipWhiteSpace();
    Token* parseNumber();
    Token* parsePeremennaya();
    char getCurrentChar() const;
    void goNextPos();
public:
    LeksichAnalizer(const std::string& inputStr);
    ~LeksichAnalizer();

    std::vector<Token*> tokenize();
};

#endif

