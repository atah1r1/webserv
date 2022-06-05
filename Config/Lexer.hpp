/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Lexer.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/05 12:30:11 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/05 13:38:04 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_HPP
#define LEXER_HPP

#include "Token.hpp"

class Lexer
{
private:
    /* data */
public:
    char c;
    unsigned int index;
    std::string str;

    Lexer(/* args */);
    Lexer(std::string str);
    ~Lexer();

    Lexer &operator=(Lexer const &rhs);
    Lexer(Lexer const &rhs);

    void advance();
    void skip_whitespace();
    Token get_next_token();
    Token advance_with_token(Token token);
    Token collect_word();

    static int is_word(char c);
};

#endif