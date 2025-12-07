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

TranslatorPolski::TranslatorPolski() {}

bool TranslatorPolski::shouldPopOperator(const Token* oper1, const Token* oper2) const
{
    if ((oper1->getType() != TokenType::OPERATION && oper1->getType() != TokenType::ONE_ARG_OPER) ||
        (oper2->getType() != TokenType::OPERATION && oper2->getType() != TokenType::ONE_ARG_OPER))
        return false;

    const TOperation* op1 = dynamic_cast<const TOperation*>(oper1);
    const TOperation* op2 = dynamic_cast<const TOperation*>(oper2);

    int p1 = op1->getPosledov();
    int p2 = op2->getPosledov();

    bool op1Unary = (oper1->getType() == TokenType::ONE_ARG_OPER);
    bool op2Unary = (oper2->getType() == TokenType::ONE_ARG_OPER);

    if (!op1Unary && op2Unary)
        return false;

    if (op1Unary && !op2Unary)
        return true;

    if (p1 < p2)
        return true;

    if (p1 == p2 && op2->isLeftAssociative())
        return true;

    return false;
}
std::vector<std::unique_ptr<Token>> TranslatorPolski::toPolishNotation(const std::vector<std::unique_ptr<Token>>& tokens)
{
    std::vector<std::unique_ptr<Token>> output;
    std::stack<std::unique_ptr<Token>> operatorsStack;

    for (const auto& token : tokens)
    {
        TokenType type = token->getType();

        if (type == TokenType::NUMBER || type == TokenType::PEREMENNAYA)
        {
            if (type == TokenType::NUMBER)
            {
                const TNumber* num = dynamic_cast<const TNumber*>(token.get());
                output.push_back(std::make_unique<TNumber>(num->getNumberValue()));
            }
            else if (type == TokenType::PEREMENNAYA)
            {
                const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(token.get());
                output.push_back(std::make_unique<TPeremennaya>(var->getPeremennaya()));
            }
        }
        else if (type == TokenType::OPERATION || type == TokenType::ONE_ARG_OPER)
        {
            const TOperation* op = dynamic_cast<const TOperation*>(token.get());

            while (!operatorsStack.empty() && \
                operatorsStack.top()->getType() != TokenType::OPEN_SKOBKA && \
                shouldPopOperator(operatorsStack.top().get(), token.get()))
            {
                TokenType topType = operatorsStack.top()->getType();
                if (topType == TokenType::OPERATION || topType == TokenType::ONE_ARG_OPER)
                {
                    const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                    output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                }
                operatorsStack.pop();
            }
            operatorsStack.push(std::make_unique<TOperation>(op->getOperation(), op->getArgument()));
        }
        else if (type == TokenType::OPEN_SKOBKA)
        {
            const TSkobki* skobka = dynamic_cast<const TSkobki*>(token.get());
            operatorsStack.push(std::make_unique<TSkobki>(skobka->getSkobka()));
        }
        else if (type == TokenType::CLOSED_SKOBKA)
        {
            while (!operatorsStack.empty() && operatorsStack.top()->getType() != TokenType::OPEN_SKOBKA)
            {
                TokenType topType = operatorsStack.top()->getType();
                if (topType == TokenType::OPERATION || topType == TokenType::ONE_ARG_OPER)
                {
                    const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                    output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                }
                operatorsStack.pop();
            }
            if (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::OPEN_SKOBKA)
            {
                operatorsStack.pop();
            }
            if (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::ONE_ARG_OPER)
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }
        }
    }

    while (!operatorsStack.empty())
    {
        TokenType topType = operatorsStack.top()->getType();
        if (topType == TokenType::OPERATION || topType == TokenType::ONE_ARG_OPER)
        {
            const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
            output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
        }
        operatorsStack.pop();
    }

    return output;
}

double TranslatorPolski::calculate(const std::vector<std::unique_ptr<Token>>& polishTokens)
{
    std::stack<double> values;

    for (const auto& token : polishTokens)
    {
        TokenType type = token->getType();

        if (type == TokenType::NUMBER)
        {
            const TNumber* num = dynamic_cast<const TNumber*>(token.get());
            values.push(num->getNumberValue());
        }
        else if (type == TokenType::OPERATION)
        {
            const TOperation* op = dynamic_cast<const TOperation*>(token.get());

            if (op->getArgument() == Arguments::TWO)
            {
                if (values.size() < 2)
                    throw std::runtime_error("TRANSLATOR_OPERATION");

                double b = values.top();
                values.pop();
                double a = values.top();
                values.pop();
                double result = 0.0;

                std::string operation = op->getOperation();

                if (operation == "+")
                    result = a + b;
                else if (operation == "-")
                    result = a - b;
                else if (operation == "*")
                    result = a * b;
                else if (operation == "/")
                {
                    if (b == 0.0)
                        throw std::runtime_error("TRANSLATOR_Divided by 0");
                    result = a / b;
                }
                else if (operation == "^")
                    result = std::pow(a, b);
                else
                    throw std::runtime_error("TRANSLATOR(BIG_ERROR): " + operation);

                values.push(result);
            }
        }
        else if (type == TokenType::ONE_ARG_OPER)
        {
            const TOperation* op = dynamic_cast<const TOperation*>(token.get());

            if (values.empty())
                throw std::runtime_error("TRANSLATOR(BIG_ERROR)_ONE_ARG");

            double a = values.top();
            values.pop();
            double result = 0.0;

            std::string operation = op->getOperation();

            if (operation == "-")
                result = -a;
            else if (operation == "exp")
                result = std::exp(a);
            else if (operation == "ln")
            {
                if (a <= 0.0)
                    throw std::runtime_error("TRANSLATOR_Ln(a<0)");
                result = std::log(a);
            }
            values.push(result);
        }
        else if (type == TokenType::PEREMENNAYA)
        {
            const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(token.get());
            throw std::runtime_error("TRANSLATOR_Perem");
        }
    }

    if (values.size() != 1)
        throw std::runtime_error("TRANSLATOR_Stack is not empty " +
            std::to_string(values.size()) + " значений");

    return values.top();
}

