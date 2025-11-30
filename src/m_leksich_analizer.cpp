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
#include "t_eof.h"

LeksichAnalizer::LeksichAnalizer(const std::string& inputStr)\
    : input(inputStr), position(0), line(1), column(1) { }
LeksichAnalizer::~LeksichAnalizer()
{
    //???
}
//
void LeksichAnalizer::goNextPos()
{
	if (position >= input.length())
		return;

	//         Column1 Column2 Column3
	//Line1:     'a'     'b'     '\n'
	//Line2:     'c'     'd'     '\n'
	if (input[position] == '\n')
	{
		line++;
		column = 1;
	}
	else
	{
		column++;
	}
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
	if (position >= input.length())
	{
		return '\0';  //END_OF_LINE
	}
	return input[position];
}
//
Token* LeksichAnalizer::parseNumber()
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
        return new TNumber(value);
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
        return new TNumber(value);
    }
}
//
Token* LeksichAnalizer::parsePeremennaya()
{
    size_t start = position;

    if (position < input.length() &&\
        (std::isalpha(input[position])))
    {
        goNextPos();

        while (position < input.length() && (std::isalpha(input[position]) ||\
                std::isdigit(input[position]) || input[position] == '_'))
        {
            goNextPos();
        }
    }

    std::string identifier = input.substr(start, position - start);
    return new TPeremennaya(identifier);
}
//
std::vector<Token*> LeksichAnalizer::tokenize()
{
    std::vector<Token*> tokens;
    position = 0;
    line = 1;
    column = 1;

    while (position < input.length())
    {
        skipWhiteSpace();
        //
        if (position >= input.length()) 
            break;
        //
        char current = getCurrentChar();
        Token* token = nullptr;
        //
        if (std::isdigit(current))
        {
            token = parseNumber();
        }
        else if (std::isalpha(current) || current == '_')
        {
            token = parsePeremennaya();
        }
        else if (current == '+' || current == '-' || current == '*' ||
            current == '/' || current == '^')
        {
            token = new TOperation(std::string(1, current));
            goNextPos();
        }
        else if (current == '(' || current == ')')
        {
            token = new TSkobki(current);
            goNextPos();
        }
        else
        {
            throw std::runtime_error("Unknown character '" + std::string(1, current)\
                + "' at line " + std::to_string(line) + ", column " + std::to_string(column));
        }
        //
        if (token)
        {
            tokens.push_back(token);
        }
    }

    tokens.push_back(new TEndOfFile());
    return tokens;
}
//
//Не забыть, делаю вот это:
//class LeksichAnalizer
//{
//    std::string input;
//    size_t position;
//    int line, column;
//    //
//    void skipWhiteSpace();
//    Token* parseNumber();
//    Token* parsePeremennaya();
//    char getCurrentChar() const;
//    void goNextPos();
//public:
//    LeksichAnalizer(const std::string& inputStr);
//    ~LeksichAnalizer();
//
//    std::vector<Token*> tokenize();
//};

