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
        return false;

    if (p1 > p2)
        return true;

    if (p1 == p2 && op2->isLeftAssociative())
        return true;

    return false;
}
std::vector<std::unique_ptr<Token>> TranslatorPolski::toPolishNotation(const std::vector<std::unique_ptr<Token>>& tokens)
{
    std::vector<std::unique_ptr<Token>> output;
    std::stack<std::unique_ptr<Token>> operatorsStack;

    auto start = tokens.begin();
    if (tokens.size() >= 2)
    {
        if (tokens[1]->getType() == TokenType::EQUATION)
        {
            start = tokens.begin() + 2;
            output.push_back(std::make_unique<TPeremennaya>\
                (dynamic_cast<const TPeremennaya*>(tokens[0].get())->getPeremennaya()));
            output.push_back(std::make_unique<TEquation>\
                (dynamic_cast<const TEquation*>(tokens[1].get())->getEquationValue()));
        }
    }
    for (auto it = start; it != tokens.end(); ++it)
    {
        const auto& token = *it;
        TokenType type = token->getType();

        if (type == TokenType::NUMBER || type == TokenType::PEREMENNAYA)
        {
            if (type == TokenType::NUMBER)
            {
                const TNumber* num = dynamic_cast<const TNumber*>(token.get());
                output.push_back(std::make_unique<TNumber>(num->getNumberValue()));
            }
            else
            {
                const TPeremennaya* var = dynamic_cast<const TPeremennaya*>(token.get());
                output.push_back(std::make_unique<TPeremennaya>(var->getPeremennaya()));
            }
            while (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::ONE_ARG_OPER)
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }
        }
        else if (type == TokenType::OPERATION || type == TokenType::ONE_ARG_OPER)
        {
            const TOperation* op = dynamic_cast<const TOperation*>(token.get());
            while (!operatorsStack.empty() &&
                operatorsStack.top()->getType() != TokenType::OPEN_SKOBKA &&
                op->getType() == TokenType::OPERATION &&
                shouldPopOperator(operatorsStack.top().get(), token.get()))
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
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
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                if (topOp->getType() == TokenType::OPERATION || topOp->getType() == TokenType::ONE_ARG_OPER)
                    output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }
            if (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::OPEN_SKOBKA)
                operatorsStack.pop();
            while (!operatorsStack.empty() && operatorsStack.top()->getType() == TokenType::ONE_ARG_OPER)
            {
                const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
                output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
                operatorsStack.pop();
            }
        }
    }
    while (!operatorsStack.empty())
    {
        const TOperation* topOp = dynamic_cast<const TOperation*>(operatorsStack.top().get());
        if (topOp->getType() == TokenType::OPERATION || topOp->getType() == TokenType::ONE_ARG_OPER)
            output.push_back(std::make_unique<TOperation>(topOp->getOperation(), topOp->getArgument()));
        operatorsStack.pop();
    }

    return output;
}

double TranslatorPolski::calculate(const std::vector<std::unique_ptr<Token>>& polishTokens, std::unordered_map<std::string, double>& perem)
{
    std::stack<double> values;

    bool isThereEquation = false;
    auto start = polishTokens.begin();
    if (polishTokens.size() >= 2)
    {
        if (polishTokens[1]->getType() == TokenType::EQUATION)
        {
            isThereEquation = true;
            start = polishTokens.begin() + 2;
        }
    }

    for (auto it = start; it != polishTokens.end(); ++it)
    {
        const auto& token = *it;
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
            std::string name = var->getPeremennaya();

            if (perem.find(name) != perem.end())
            {
                values.push(perem[name]);
            }
        }
    }

    if (values.size() != 1)
        throw std::runtime_error("TRANSLATOR_Stack is not empty " +
            std::to_string(values.size()) + " значений");

    if (isThereEquation)
    {
        perem[dynamic_cast<const TPeremennaya*>(polishTokens[0].get())->getPeremennaya()] = values.top();
    }

    return values.top();
}

