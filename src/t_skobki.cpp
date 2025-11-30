#include "t_skobki.h"

TSkobki::TSkobki(char skobka)
{
	if (skobka == '(')
		open = true;
	else 
		open = false;
}
TokenType TSkobki::getType() const
{
	return TokenType::SKOBKI;
}
char TSkobki::getSkobka() const
{
	return skobka;
}
bool TSkobki::isOpenSkobka() const
{
	return open;
}

