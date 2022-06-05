/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 12:32:42 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/05 12:40:38 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Token.hpp"

Token::Token(/* args */)
{
    this->type = TOKEN_EOF;
    this->value = "\0";
}

Token::Token(TokenType type, std::string value)
{
    this->type = type;
    this->value = value;
}

Token::Token(const Token &rhs)
{
    *this = rhs;
}

Token &Token::operator=(Token const &rhs)
{
    this->type = rhs.type;
    this->value = rhs.value;
    return *this;
}

Token::~Token()
{
}
