#ifndef __LEXER_HPP__
#define __LEXER_HPP__

#include "Token.hpp"

class Lexer
{
    public:
        /* data */
        char			c;
        unsigned int	i;
        std::string		contents;

        // Constructors
        Lexer(std::string contents);
        Lexer();
        
        // Operations
        Lexer(const Lexer& lexer);
        Lexer& operator=(const Lexer& lexer);

        // Member Methods
        void    advance();
        void	skipWhiteSpaces();
        Token   getNextToken();
        Token   collectWord();
        Token   advanceWithToken(Token token);

        // Non Member Methods
        static int	isWord(char c);
};

#endif