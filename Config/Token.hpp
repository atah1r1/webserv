/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Token.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 12:32:34 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/05 12:36:29 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <iostream>

enum TokenType
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
private:
    /* data */
public:
    TokenType type;
    std::string value;

    Token(/* args */);
    Token(TokenType type, std::string value);
    Token(Token const &rhs);
    Token &operator=(Token const &rhs);
    ~Token();
};

#endif
