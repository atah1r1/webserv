#include "include/Token.hpp"

Token::Token()
{
    this->type = TOKEN_EOF;
    this->value = "\0";
}

Token::Token(TypeToken type, std::string value)
{
    this->type = type;
    this->value = value;
}

Token& Token::operator=(const Token& token)
{
    this->type = token.type;
    this->value = token.value;
    return (*this);
}

Token::Token(const Token& token)
{
    *this = token;
}
