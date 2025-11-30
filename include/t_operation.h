#ifndef __T_OPERATION__
#define __T_OPERATION__

#include <string>

#include "token.h"

class TOperation : public Token
{
    std::string oper;
    int posledov;
public:
    TOperation(std::string operation);
    TokenType getType() const override;
    std::string getOperation() const;
    int getPosledov() const;
    bool isLeftAssociative() const;
};

#endif

