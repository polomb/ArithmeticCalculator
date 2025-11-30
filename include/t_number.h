#ifndef __T_NUMBER__
#define __T_NUMBER__

#include <string>

#include "token.h"

class TNumber : public Token
{
    double value;
public:
    TNumber(double val);
    TokenType getType() const override;
    double getNumberValue() const;
};

#endif

