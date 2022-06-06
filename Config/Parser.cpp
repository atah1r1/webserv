/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 09:38:49 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/06 18:18:29 by atahiri          ###   ########.fr       */
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

std::vector<ServerConfig> Parser::parse()
{
    std::vector<ServerConfig> servers;
    this->grabServer(); // check if is server context and advance to next token
    servers.push_back(parseServer());

    int count_server = 0;
    while (current_token.type != TOKEN_EOF)
    {
        this->grabServer();
        servers.push_back(parseServer());
        count_server++;
    }
    return (servers);
}

std::string Parser::parseIp()
{
    this->grab(WORD); // ip and port
    std::string ip;
    if (current_token.type == WORD)
    {
        ip = current_token.value;
        this->grab(WORD);
    }
    else
    {
        std::cout << "Error: expected ip but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (ip);
}

ServerConfig Parser::parseServer()
{
    ServerConfig server_setup;

    this->grab(OPEN_BRACKET);
    while (current_token.type != CLOSE_BRACKET && current_token.type != TOKEN_EOF && current_token.type != TOKEN_ERR)
    {
        if (!current_token.value.compare("listen"))
        {
            // server_setup.setServerIp(parseIp());
            // this->grab(WORD);
            std::cout << parseIp() << std::endl;
        }
        // else if (!current_token.value.compare("server_name") && server_setup.server_name.empty())
        //     server_setup.server_name = parseWords();
        // else if (!current_token.value.compare("root") && !server_setup.root.length())
        //     server_setup.root = parseWord();
        // else if (!current_token.value.compare("index") && server_setup.index.empty())
        //     server_setup.index = parseWords();
        // else if (!current_token.value.compare("error_pages") && server_setup.error_pages.empty())
        //     server_setup.error_pages = parseErrorPages();
        // else if (!current_token.value.compare("client_max_body_size") && server_setup.client_max_body_size == -1)
        //     server_setup.client_max_body_size = stringToInt(parseWord());
        // else if (!current_token.value.compare("request_method") && server_setup.request_method.empty())
        //     server_setup.request_method = parseWords();
        // else if (!current_token.value.compare("autoindex") && !server_setup.autoindex.length())
        //     server_setup.autoindex = parseWord();
        // else if (!current_token.value.compare("location"))
        // {
        //     server_setup.locations.push_back(parseLocation());
        //     continue;
        // }
        // else
        // {
        //     std::cout << "Invalid Token" << std::endl;
        // }
        this->grab(SEMICOLON);
    }
    this->grab(CLOSE_BRACKET);
    // return (CheckConfig(server_setup));
    return (server_setup);
}