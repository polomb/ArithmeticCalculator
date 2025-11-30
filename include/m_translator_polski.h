#ifndef __M_TRANSLATOR_POLSKI__
#define __M_TRANSLATOR_POLSKI__

#include <vector>
#include "token.h"

class TranslatorPolski
{
    bool shouldPopOperator(const Token* oper1, const Token* oper2) const;
    void clearTokens(std::vector<Token*>& tokens);
public:
    TranslatorPolski();
    ~TranslatorPolski();

    std::vector<Token*> toPolishNotation(const std::vector<Token*>& tokens);

    double calculate(const std::vector<Token*>& polishTokens);
};

#endif

