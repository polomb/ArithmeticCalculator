#ifndef __T_END_OF_FILE__//EOF
#define __T_END_OF_FILE__

#include "token.h"

class TEndOfFile : public Token
{
public:
    TEndOfFile();
    TokenType getType() const override;
};

#endif

