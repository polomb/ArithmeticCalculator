#include "m_leksich_analizer.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <cctype> //For isdigit and isalpha

#include "token.h"
#include "t_number.h"
#include "t_operation.h"
#include "t_peremennaya.h"
#include "t_skobki.h"

LeksichAnalizer::LeksichAnalizer(const std::string& inputStr)\
    : input(inputStr), position(0) {}
//
void LeksichAnalizer::goNextPos()
{
	if (position >= input.length())
		return;

	position++;
}
//
void LeksichAnalizer::skipWhiteSpace()
{
	while (position < input.length() && (input[position] == ' ' || input[position] == '\n'))
	{
		goNextPos();
	}
}
//
char LeksichAnalizer::getCurrentChar() const
{
	return input[position];
}
//
std::unique_ptr<Token> LeksichAnalizer::parseNumber()
{
	size_t start = position;
	bool dot = false;

    while (position < input.length())
    {
        char c = input[position];
        if (std::isdigit(c))
        {
            goNextPos();
        }
        else if (c == '.' && !dot)
        {
            dot = true;
            goNextPos();
        }
        else
        {
            break;
        }
    }
    std::string numStr = input.substr(start, position - start);

    if (!dot)
    {
        //int
        int value = 0;
        for (char c : numStr)
        {
            value = value * 10 + (c - '0');
        }
        return std::make_unique<TNumber>(value);
    }
    else
    {
        //double
        size_t dotPos = numStr.find('.');
        std::string intPart = numStr.substr(0, dotPos);
        std::string fracPart = numStr.substr(dotPos + 1);

        int integer = 0;
        for (size_t i = 0; i < intPart.length(); i++)
        {
            char c = intPart[i];
            integer = integer * 10 + (c - '0');
        }

        double fraction = 0.0;
        for (size_t i = 0; i < fracPart.length(); i++)
        {
            char c = fracPart[i];
            fraction = fraction * 10 + (c - '0');
        }
        for (size_t i = 0; i < fracPart.length(); i++)
        {
            fraction /= 10.0;
        }

        double value = integer + fraction;
        return std::make_unique<TNumber>(value);
    }
}
//
std::unique_ptr<Token> LeksichAnalizer::parsePeremOrOneArgOper()
{
    size_t start = position;
    bool isPeremennaya = true;

    if (position < input.length() && std::isalpha(input[position]))
    {
        goNextPos();

        while (position < input.length() && (std::isalpha(input[position]) ||\
                std::isdigit(input[position]) || input[position] == '_' || input[position] == '('))
        {
            if (input[position] == '(')
            {
                isPeremennaya = false;
                break;
            }
            else
            {
                goNextPos();
            }
        }
    }
    std::string identifier = input.substr(start, position - start);
    if (isPeremennaya)
    {
        return std::make_unique<TPeremennaya>(identifier);
    }
    else
    {
        if (identifier == "exp" || identifier == "ln")
        {
            return std::make_unique<TOperation>(identifier, Arguments::ONE);
        }
        else
        {
            return std::make_unique<TPeremennaya>(identifier);
        }
    }
}
//
std::vector<std::unique_ptr<Token>> LeksichAnalizer::tokenize()
{
    std::vector<std::unique_ptr<Token>> tokens;
    position = 0;

    while (position < input.length())
    {
        skipWhiteSpace();
        //
        if (position >= input.length()) 
            break;
        //
        char current = getCurrentChar();
        std::unique_ptr<Token> token = nullptr;
        //
        if (std::isdigit(current) || current == '.')
        {
            token = parseNumber();
        }
        else if (std::isalpha(current))
        {
            token = parsePeremOrOneArgOper();
        }
        else if (current == '+' || current == '-' || current == '*' ||
            current == '/' || current == '^')
        {
            token = std::make_unique<TOperation>(std::string(1, current), Arguments::TWO);
            goNextPos();
        }
        else if (current == '(' || current == ')')
        {
            token = std::make_unique<TSkobki>(current);
            goNextPos();
        }
        else if(current == '=')
        {
            token = std::make_unique<TEquation>(current);
            goNextPos();
        }
        else
        {
            throw std::runtime_error("LEKSICH_Unknown character '" + std::string(1, current)\
                + "' at position " + std::to_string(position+1));
        }
        //
        if (token)
        {
            tokens.push_back(std::move(token));
        }
    }
    if (tokens.size() == 2)
    {
        TokenType fir = tokens[0]->getType();
        TokenType sec = tokens[1]->getType();
        if (fir == TokenType::OPERATION && (sec == TokenType::NUMBER || sec == TokenType::PEREMENNAYA))
        {
            TOperation* operation = dynamic_cast<TOperation*>(tokens[0].get());
            if (operation->getOperation() == "-")
            {
                operation->setArgument(Arguments::ONE);
            }
        }
    }
    else
    {
        TokenType fir = tokens[0]->getType();
        TokenType sec = tokens[1]->getType();
        if (fir == TokenType::OPERATION && (sec == TokenType::NUMBER || sec == TokenType::PEREMENNAYA))
        {
            TOperation* operation = dynamic_cast<TOperation*>(tokens[0].get());
            if (operation->getOperation() == "-")
            {
                operation->setArgument(Arguments::ONE);
            }
        }
        for (size_t i = 2; i < tokens.size(); i++)
        {
            TokenType fir = tokens[i-2]->getType();
            TokenType sec = tokens[i-1]->getType();
            TokenType thi = tokens[i]->getType();
            if ((thi == TokenType::NUMBER || thi == TokenType::PEREMENNAYA ||thi == TokenType::OPEN_SKOBKA || thi == TokenType::ONE_ARG_OPER)\
                && (sec == TokenType::OPERATION) && \
                (fir != TokenType::NUMBER && fir != TokenType::PEREMENNAYA && fir != TokenType::CLOSED_SKOBKA))
            {
                if (dynamic_cast<TOperation*>(tokens[i - 1].get())->getOperation() == "-")
                {
                    dynamic_cast<TOperation*>(tokens[i - 1].get())->setArgument(Arguments::ONE);
                }
            }
        }
    }

    return tokens;
}

