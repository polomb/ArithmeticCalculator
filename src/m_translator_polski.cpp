#include "m_translator_polski.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <stack>

#include "token.h"
#include "t_number.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"
#include "t_eof.h"

TranslatorPolski::TranslatorPolski() {}
TranslatorPolski::~TranslatorPolski() {}

bool TranslatorPolski::shouldPopOperator(const Token* oper1, const Token* oper2) const
{
    if (oper1->getType() != TokenType::OPERATION || oper2->getType() != TokenType::OPERATION)
    {
        return false;
    }

    const TOperation* operation1 = dynamic_cast<const TOperation*>(oper1);
    const TOperation* operation2 = dynamic_cast<const TOperation*>(oper2);

    int posledovatelnost1 = operation1->getPosledov();
    int posledovatelnost2 = operation2->getPosledov();

    if (posledovatelnost1 > posledovatelnost2)
    {
        return true;
    }
    else if (posledovatelnost1 == posledovatelnost2)
    {
        return operation1->getOperation() != "^";
    }

    return false;
}

void TranslatorPolski::clearTokens(std::vector<Token*>& tokens)
{
    for (Token* token : tokens)
    {
        delete token;
    }
    tokens.clear();
}

std::vector<Token*> TranslatorPolski::toPolishNotation(const std::vector<Token*>& tokens)
{
    std::vector<Token*> output;
    std::stack<Token*> operStack;

    for (const Token* token : tokens)
    {
        TokenType type = token->getType();

        if (type == TokenType::NUMBER)
        {
            const TNumber* number = dynamic_cast<const TNumber*>(token);
            output.push_back(new TNumber(number->getNumberValue()));
        }
        else if (type == TokenType::PEREMENNAYA)
        {
            const TPeremennaya* variable = dynamic_cast<const TPeremennaya*>(token);
            output.push_back(new TPeremennaya(variable->getPeremennaya()));
        }
        else if (type == TokenType::OPERATION)
        {
            const TOperation* operation = dynamic_cast<const TOperation*>(token);
            TOperation* operCopy = new TOperation(operation->getOperation());

            while (!operStack.empty() && operStack.top()->getType() == TokenType::OPERATION\
                && shouldPopOperator(operStack.top(), operCopy))
            {

                TOperation* operTop = dynamic_cast<TOperation*>(operStack.top());
                output.push_back(new TOperation(operTop->getOperation()));
                delete operStack.top();
                operStack.pop();
            }
            operStack.push(operCopy);
        }
        else if (type == TokenType::SKOBKI)
        {
            const TSkobki* bracket = dynamic_cast<const TSkobki*>(token);
            if (bracket->isOpenSkobka())
            {
                operStack.push(new TSkobki('('));
            }
            else
            {
                while (!operStack.empty() && operStack.top()->getType() != TokenType::SKOBKI)
                {

                    TOperation* topOp = dynamic_cast<TOperation*>(operStack.top());
                    output.push_back(new TOperation(topOp->getOperation()));
                    delete operStack.top();
                    operStack.pop();
                }

                if (operStack.empty())
                {
                    clearTokens(output);
                    throw std::runtime_error("Mismatched brackets");
                }

                delete operStack.top();
                operStack.pop();
            }
        }
    }
    while (!operStack.empty())
    {
        Token* topToken = operStack.top();
        if (topToken->getType() == TokenType::SKOBKI)
        {
            clearTokens(output);
            throw std::runtime_error("Mismatched brackets");
        }

        TOperation* operTop = dynamic_cast<TOperation*>(topToken);
        output.push_back(new TOperation(operTop->getOperation()));
        delete operStack.top();
        operStack.pop();
    }
    output.push_back(new TEndOfFile());

    return output;
}

double TranslatorPolski::calculate(const std::vector<Token*>& polishTokens) {
    std::stack<double> stack;

    for (Token* token : polishTokens)
    {
        if (token->getType() == TokenType::NUMBER)
        {
            TNumber* number = dynamic_cast<TNumber*>(token);
            stack.push(number->getNumberValue());
        }
        else if (token->getType() == TokenType::OPERATION)
        {
            if (stack.size() < 2)
            {
                throw std::runtime_error("Not enough operands for operation");
            }

            double right = stack.top();
            stack.pop();
            double left = stack.top();
            stack.pop();

            TOperation* operation = dynamic_cast<TOperation*>(token);
            std::string op = operation->getOperation();
            double result = 0.0;

            if (op == "+")
            {
                result = left + right;
            }
            else if (op == "-")
            {
                result = left - right;
            }
            else if (op == "*")
            {
                result = left * right;
            }
            else if (op == "/")
            {
                if (right == 0)
                {
                    throw std::runtime_error("Division by zero");
                }
                result = left / right;
            }
            else if (op == "^")
            {
                result = std::pow(left, right);
            }
            else
            {
                throw std::runtime_error("Unknown operation: " + op);
            }

            stack.push(result);
        }
        else if (token->getType() == TokenType::PEREMENNAYA)
        {
            throw std::runtime_error("Variables are not supported in calculation");
        }
    }

    if (stack.size() != 1)
    {
        throw std::runtime_error("Invalid expression");
    }

    return stack.top();
}

//Не забыть, делаю вот это:
//class TranslatorPolski
//{
//    bool shouldPopOperator(const Token* oper1, const Token* oper2) const;
//    void clearTokens(std::vector<Token*>& tokens);
//public:
//    TranslatorPolski();
//    ~TranslatorPolski();
//
//    std::vector<Token*> toPolishNotation(const std::vector<Token*>& tokens);
//
//    double calculate(const std::vector<Token*>& polishTokens);
//};

