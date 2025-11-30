#ifndef __TOKEN__
#define __TOKEN__

#include <string>

enum class TokenType
{
    NUMBER,
    OPERATION,
    PEREMENNAYA,
    SKOBKI,
    L_VALUE,
    R_VALUE,
    EOF_TOKEN,
    ERROR
};

class Token
{
public:
    virtual ~Token() = default;
    virtual TokenType getType() const = 0;
};

#endif

