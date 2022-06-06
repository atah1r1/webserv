#ifndef __TOKEN_HPP__
#define __TOKEN_HPP__

#include <iostream>

enum  TypeToken
{
    WORD,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    SEMICOLON,

    TOKEN_COLON,
    TOKEN_BODY,

    TOKEN_ERR,
    TOKEN_EOF
};

class Token
{
    public:
        /* data */
        TypeToken type;
        std::string value;

        Token();
        Token(TypeToken type, std::string value);
        Token(const Token& token);
        Token& operator=(const Token& token);
};


#endif