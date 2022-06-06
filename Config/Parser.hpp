/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 09:38:10 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/06 11:42:08 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"

class Parser
{
private:
    /* data */
public:
    Lexer lexer;
    Token current_token;
    Token previous_token;
    
    
    Parser(Lexer lexer);
    Parser(const Parser &parser);
    Parser &operator=(const Parser &parser);
    ~Parser();

    void grab(TokenType token);
    int grabServer();
};



#endif