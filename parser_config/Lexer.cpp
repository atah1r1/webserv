#include "include/Lexer.hpp"

// Constructors
Lexer::Lexer(std::string contents)
{
    this->contents = contents;
    this->i = 0;
    this->c = contents[this->i];
}

Lexer::Lexer()
{
    this->contents = "";
    this->i = 0;
    this->c = contents[this->i];
}

// Operations
Lexer::Lexer(const Lexer& lexer)
{
    *this = lexer;
}

Lexer& Lexer::operator=(const Lexer& lexer)
{
    this->contents = lexer.contents;
    this->c = lexer.c;
    this->i = lexer.i;
    return (*this);
}

// Member Functions
void    Lexer::advance()
{
    if (this->c != '\0' && this->i < this->contents.length())
	{
		this->i += 1;
		this->c = this->contents[this->i];
	}
}

void	Lexer::skipWhiteSpaces()
{
    while (this->c == ' ' || this->c == '\t' || this->c == '\n')
		advance();
}

Token   Lexer::getNextToken()
{
    while (this->c != '\0' && this->i < this->contents.length())
	{
		if (this->c == ' ' || this->c == '\t' || this->c == '\n')
			skipWhiteSpaces();
		else if (this->c == '{')
			return (advanceWithToken(Token(OPEN_BRACKET, "{")));
		else if (this->c == '}')
			return (advanceWithToken(Token(CLOSE_BRACKET, "}")));
        else if (this->c == ';')
			return (advanceWithToken(Token(SEMICOLON, ";")));
        else
            return (collectWord());
	}
	return (Token(TOKEN_EOF, "\0"));
}

Token   Lexer::collectWord()
{
    std::string value;

    while(isWord(this->c))
    {
        value.push_back(this->c);
        advance();
    }
    return (Token(WORD, value));
}

Token   Lexer::advanceWithToken(Token token)
{
    advance();
	return (token);
}

int	Lexer::isWord(char c)
{
	if (c != '{' && c != '}' && c != ' ' && c != ';'
        && c != '\n' && c != '\0' && c != '\t')
		return (1);
	return (0);
}