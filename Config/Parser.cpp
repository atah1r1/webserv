/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 09:38:49 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/06 12:33:47 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <iostream>

Parser::Parser(Lexer lexer) : lexer(lexer)
{
    this->current_token = this->lexer.get_next_token();
    this->previous_token = this->current_token;
}

Parser::Parser(const Parser &parser)
{
    *this = parser;
}

Parser &Parser::operator=(const Parser &parser)
{
    this->lexer = parser.lexer;
    this->current_token = parser.current_token;
    this->previous_token = parser.previous_token;
    return *this;
}

Parser::~Parser()
{
}

void Parser::grab(TokenType token)
{
    if (this->current_token.type == token)
    {
        this->previous_token = this->current_token;
        this->current_token = this->lexer.get_next_token();
    }
    else
    {
        std::cout << "Error: expected " << token << " but got " << this->current_token.type << std::endl;
        exit(1);
    }
}


int Parser::grabServer()
{
    if (current_token.value.compare("server"))
    {
        std::cout << "Invalid Context" << std::endl;
        exit(1);
    }
    this->grab(WORD); // server
    return (0);
}