#include "t_number.h"

TNumber::TNumber(double val) : value(val)
{

}
TokenType TNumber::getType() const
{
	return TokenType::NUMBER;
}
double TNumber::getNumberValue() const
{
	return value;
}

