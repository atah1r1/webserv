/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 12:30:13 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/06 09:45:55 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Lexer.hpp"

Lexer::Lexer(/* args */)
{
    this->str = "";
    this->index = 0;
    this->c = this->str[this->index];
}


Lexer::Lexer(std::string str)
{
    this->str = str;
    this->index = 0;
    this->c = this->str[this->index];
}

Lexer::Lexer(Lexer const &rhs)
{
    *this = rhs;
}

Lexer &Lexer::operator=(Lexer const &rhs)
{
    this->str = rhs.str;
    this->index = rhs.index;
    this->c = this->str[this->index];
    return *this;
}

void Lexer::advance()
{
    if (this->c != '\0' && this->index <= this->str.length())
    {
        this->index++;
        this->c = this->str[this->index];
    }
}

void Lexer::skip_whitespace()
{
    while (this->c == ' ' || this->c == '\t' || this->c == '\n' || this->c == '\r')
    {
        this->advance();
    }
}

Token Lexer::get_next_token()
{
    while (this->c != '\0' && this->index < this->str.length())
    {
        if (this->c == ' ' || this->c == '\t' || this->c == '\n')
            this->skip_whitespace();
        else if (this->c == '{')
            return (this->advance_with_token(Token(OPEN_BRACKET, "{")));
        else if (this->c == '}')
            return (this->advance_with_token(Token(CLOSE_BRACKET, "}")));
        else if (this->c == ';')
            return (this->advance_with_token(Token(SEMICOLON, ";")));
        else
            return (this->collect_word());
    }
    return (Token(TOKEN_EOF, "\0"));
}

Token Lexer::advance_with_token(Token const & token)
{
    this->advance();
    return (token);
}

int	Lexer::is_word(char c)
{
	if (c != '{' && c != '}' && c != ' ' && c != ';'
        && c != '\n' && c != '\0' && c != '\t')
		return (1);
	return (0);
}

Token Lexer::collect_word()
{
    std::string value;

    while (is_word(this->c))
    {
        value += this->c;
        advance();
    }
    return (Token(WORD, value));
}


Lexer::~Lexer()
{
}

int main()
{
    return 0;
}