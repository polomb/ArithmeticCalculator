#include "m_sintaksis_analizer.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <cctype> //For isdigit and isalpha

#include "token.h"
#include "t_number.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"
#include "t_eof.h"

SintaksisAnalizer::SintaksisAnalizer() {}
SintaksisAnalizer::~SintaksisAnalizer() {}

bool SintaksisAnalizer::isCorrect(const std::vector<Token*>& tokens)
{
    int bracketBalance = 0;

    for (size_t i = 1; i < tokens.size(); i++) // i от 1 -> prev всегда есть
    {
        TokenType prev = tokens[i - 1]->getType();
        TokenType cur = tokens[i]->getType();
        //Нужно обдумать все возможные операции (-_-)
        if (cur == TokenType::OPERATION && prev == TokenType::OPERATION) // -- or ++ or +- or ...
        {
            throw std::runtime_error("Two operators in a row");
        }
        if (cur == TokenType::SKOBKI && prev == TokenType::SKOBKI) // () or )(
        {
            TSkobki* bracket1 = dynamic_cast<TSkobki*>(tokens[i - 1]);
            TSkobki* bracket2 = dynamic_cast<TSkobki*>(tokens[i]);
            if (bracket1->isOpenSkobka() && !bracket1->isOpenSkobka())
            {
                throw std::runtime_error("Skobki with no number: ()");
            }
            if (!bracket1->isOpenSkobka() && bracket1->isOpenSkobka())
            {
                throw std::runtime_error("Skobki with no operation: )(");
            }
        }
        if (cur == TokenType::SKOBKI && prev == TokenType::NUMBER) // num(
        {
            TSkobki* bracket = dynamic_cast<TSkobki*>(tokens[i]);
            if (bracket->isOpenSkobka())
            {
                throw std::runtime_error("Number before opening bracket: num(");
            }
        }
        if (cur == TokenType::NUMBER && prev == TokenType::SKOBKI) // )num
        {
            TSkobki* bracket = dynamic_cast<TSkobki*>(tokens[i - 1]);
            if (!bracket->isOpenSkobka())
            {
                throw std::runtime_error("Number after closing bracket: )num");
            }
        }

    }

    if (bracketBalance != 0)
    {
        throw std::runtime_error("Unbalanced brackets");
    }

    return true;
}

//Не забыть, делаю вот это:
//class SintaksisAnalizer
//{
//public:
//    SintaksisAnalizer();
//    ~SintaksisAnalizer();
//
//    bool isCorrect(const std::vector<Token*>& tokens);
//};

