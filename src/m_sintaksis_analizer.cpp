#include "m_sintaksis_analizer.h"

#include <string>//For string and +
#include <vector>//For vector and +
#include <stdexcept>
#include <cctype> //For isdigit and isalpha
#include <stack> //For stack and +
#include <algorithm> //For find

#include "token.h"
#include "t_number.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"

SintaksisAnalizer::SintaksisAnalizer() {}

bool SintaksisAnalizer::isCorrect(const std::vector<std::unique_ptr<Token>>& tokens, std::unordered_map<std::string, double>& perem)
{
    if (tokens.size() == 1)
    {
        TokenType cur = tokens[0]->getType();
        if(cur == TokenType::NUMBER || cur == TokenType::PEREMENNAYA)
        {
            if (cur == TokenType::PEREMENNAYA)
            {
                const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(tokens[0].get());
                if (perem.find(var->getPeremennaya()) != perem.end())
                {

                }
                else
                    throw std::runtime_error("There is no such perem");
            }
        }
        else
        {
            throw std::runtime_error("One token can be only number or peremennaya");
        }
    }
    else
    {
        for (size_t i = 1; i < tokens.size(); i++) // i от 1 -> prev always exists
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
            else if (prev == TokenType::ONE_ARG_OPER)//ONE_ARG_OP: NUM, PEREM, (, ONE_ARG_OP
            {
                if (cur == TokenType::NUMBER || cur == TokenType::PEREMENNAYA\
                    || cur == TokenType::OPEN_SKOBKA || cur == TokenType::ONE_ARG_OPER)
                {

                }
                else
                    throw std::runtime_error("Sintaksis_ONE_ARG_OPER");
            }
            else if (prev == TokenType::PEREMENNAYA) //PEREM: OPER, ), EQUATION
            {
                const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(tokens[i-1].get()); // i-1 -> prev
                if (i - 1 != 0)
                {
                    if (perem[var->getPeremennaya()])
                    {

                    }
                    else
                        throw std::runtime_error("There is no such perem");
                }
                //
                if (cur == TokenType::OPERATION || cur == TokenType::CLOSED_SKOBKA || \
                    cur == TokenType::EQUATION)
                {
                    if (cur == TokenType::EQUATION)
                    {
                        if (tokens.size() > 2)
                        {

                        }
                        else
                            throw std::runtime_error("Sintaksis_PEREMENNAYA_2");
                    }
                }
                else
                    throw std::runtime_error("Sintaksis_PEREMENNAYA_1");
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
            else if (prev == TokenType::EQUATION)//= -> expected to be on [1], and on [0] expected oper
            {
                if (i - 1 == 1) // i - 1 <- place for prev
                {
                    if (cur == TokenType::NUMBER || cur == TokenType::PEREMENNAYA\
                        || cur == TokenType::OPEN_SKOBKA || cur == TokenType::ONE_ARG_OPER)
                    {
                        // =: NUM, PEREM, (, ONE_ARG_OP
                    }
                    else
                        throw std::runtime_error("Sintaksis_EQUATION_3");
                    if (tokens[0]->getType() == TokenType::PEREMENNAYA)
                    {
                        // Should be oper = ... If not -> error
                    }
                    else
                        throw std::runtime_error("Sintaksis_EQUATION_2");
                }
                else
                    throw std::runtime_error("Sintaksis_EQUATION_1");
            }
        }
    }
    //
    //SKOBKI at right positions
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

