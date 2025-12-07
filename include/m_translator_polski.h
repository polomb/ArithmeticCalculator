#ifndef __M_TRANSLATOR_POLSKI__
#define __M_TRANSLATOR_POLSKI__

#include <vector>
#include <memory>

#include "token.h"

class TranslatorPolski
{
    bool shouldPopOperator(const Token* oper1, const Token* oper2) const;
public:
    TranslatorPolski();

    std::vector<std::unique_ptr<Token>> toPolishNotation(const std::vector<std::unique_ptr<Token>>& tokens);

    double calculate(const std::vector<std::unique_ptr<Token>>& polishTokens);
};

#endif

