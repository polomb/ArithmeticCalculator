#include "t_operation.h"

TOperation::TOperation(std::string operation) : oper(operation) 
{
    switch (operation[0])
    {
    case '+': case '-': 
        posledov = 1; 
        break;
    case '*': case '/': 
        posledov = 2; 
        break;
    case '^': 
        posledov = 3; 
        break;
    default: 
        posledov = 0;
    }
}
TokenType TOperation::getType() const 
{
    return TokenType::OPERATION;
}
std::string TOperation::getOperation() const
{
    return oper;
}
int TOperation::getPosledov() const
{
    return posledov;
}
bool TOperation::isLeftAssociative() const
{
    return oper != std::string(1, '^');
}

