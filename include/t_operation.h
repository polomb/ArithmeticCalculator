#ifndef __T_OPERATION__
#define __T_OPERATION__

#include <string>

#include "token.h"

enum class Arguments
{
    TWO,
    ONE
};

class TOperation : public Token
{
    std::string oper;
    Arguments argum;
    int posledov;
public:
    TOperation(std::string operation, Arguments argument);
    TokenType getType() const override;
    std::string getOperation() const;
    int getPosledov() const;
    Arguments getArgument() const;
    void setArgument(Arguments argument);
    bool isLeftAssociative() const;
};

#endif

