#include "m_sintaksis_analizer.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <cctype> //For isdigit and isalpha
#include <stack>

#include "token.h"
#include "t_number.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"

SintaksisAnalizer::SintaksisAnalizer() {}

bool SintaksisAnalizer::isCorrect(const std::vector<std::unique_ptr<Token>>& tokens)
{
    if (tokens.size() == 1)
    {
        TokenType cur = tokens[0]->getType();
        if(cur == TokenType::NUMBER || cur == TokenType::PEREMENNAYA) {}
        else
        {
            throw std::runtime_error("One token can be only number or peremennaya");
        }
    }
    else
    {
        for (size_t i = 1; i < tokens.size(); i++) // i от 1 -> prev всегда есть
        {
            TokenType prev = tokens[i - 1]->getType();
            TokenType cur = tokens[i]->getType();
            if (prev == TokenType::NUMBER) //NUM: OPER, )
            {
                if (cur == TokenType::OPERATION || cur == TokenType::CLOSED_SKOBKA)
                {

                }
                else
                    throw std::runtime_error("Sintaksis_NUMBER");
            }
            else if (prev == TokenType::OPERATION) //OPER: NUM, PEREM, (, ), ONE_ARG_OP
            {
                if (cur == TokenType::NUMBER || cur == TokenType::PEREMENNAYA\
                    || cur == TokenType::OPEN_SKOBKA || cur == TokenType::ONE_ARG_OPER)
                {

                }
                else
                    throw std::runtime_error("Sintaksis_OPERATION");
            }
            else if (prev == TokenType::ONE_ARG_OPER)//ONE_ARG_OP: NUM, PEREM, (, 
            {
                if (cur == TokenType::NUMBER || cur == TokenType::PEREMENNAYA\
                    || cur == TokenType::OPEN_SKOBKA || cur == TokenType::ONE_ARG_OPER)
                {

                }
                else
                    throw std::runtime_error("Sintaksis_ONE_ARG_OPER");
            }
            else if (prev == TokenType::PEREMENNAYA) //PEREM: OPER, )
            {
                if (cur == TokenType::OPERATION || cur == TokenType::CLOSED_SKOBKA)
                {

                }
                else
                    throw std::runtime_error("Sintaksis_PEREMENNAYA");
            }
            else if (prev == TokenType::OPEN_SKOBKA) //(: NUM, PEREM, ONE_ARG_OP, (
            {
                if (cur == TokenType::NUMBER || cur == TokenType::PEREMENNAYA\
                    || cur == TokenType::ONE_ARG_OPER || cur == TokenType::OPEN_SKOBKA)
                {

                }
                else
                    throw std::runtime_error("Sintaksis_OPEN_SKOBKA");
            }
            else if (prev == TokenType::CLOSED_SKOBKA)//): OPER, )
            {
                if (cur == TokenType::OPERATION || cur == TokenType::CLOSED_SKOBKA)
                {

                }
                else
                    throw std::runtime_error("Sintaksis_CLOSED_SKOBKA");
            }
        }
    }
    //
    //SKOBKI at right possitions
    std::stack<TokenType> stack;

    for (const auto& token : tokens)
    {
        if (token->getType() == TokenType::OPEN_SKOBKA)
        {
            stack.push(TokenType::OPEN_SKOBKA);
        }
        else if (token->getType() == TokenType::CLOSED_SKOBKA)
        {
            if (stack.empty() || stack.top() != TokenType::OPEN_SKOBKA)
            {
                throw std::runtime_error("Sintaksis_SKOBKI_NESOGLASOVANNI");
            }
            stack.pop();
        }
    }
    return true;
}

