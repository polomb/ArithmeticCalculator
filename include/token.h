#ifndef __TOKEN__
#define __TOKEN__

#include <string>

enum class TokenType
{
    NUMBER,
    OPERATION,
    ONE_ARG_OPER,
    PEREMENNAYA,
    OPEN_SKOBKA,
    CLOSED_SKOBKA
};

class Token
{
public:
    virtual ~Token() = default;
    virtual TokenType getType() const = 0;
};

#endif

