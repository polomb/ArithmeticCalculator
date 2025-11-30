#include "t_eof.h"

TEndOfFile::TEndOfFile()
{

}
TokenType TEndOfFile::getType() const
{
	return TokenType::EOF_TOKEN;
}

