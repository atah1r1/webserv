/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 09:38:49 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/07 16:29:33 by atahiri          ###   ########.fr       */
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
std::string Parser::parseServerName()
{
    this->grab(WORD); // server_name
    std::string server_name;
    if (current_token.type == WORD)
    {
        server_name = current_token.value;
        this->grab(WORD);
    }
    else
    {
        std::cout << "Error: expected ip but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (server_name);
}

std::string Parser::parseRoot()
{
    this->grab(WORD); // root
    std::string root;
    if (current_token.type == WORD)
    {
        root = current_token.value;
        this->grab(WORD);
    }
    else
    {
        std::cout << "Error: expected ip but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (root);
}

std::vector<std::string> Parser::parseIndex()
{
    std::vector<std::string> index;
    this->grab(WORD); // index
    while (current_token.type == WORD)
    {
        // std::cout << "---> " << current_token.value << std::endl;
        index.push_back(current_token.value);
        this->grab(WORD);
    }
    return (index);
}

std::vector<std::string> Parser::parseErrorPages()
{
    std::vector<std::string> index;
    this->grab(WORD); // index
    while (current_token.type == WORD)
    {
        // std::cout << "---> " << current_token.value << std::endl;
        index.push_back(current_token.value);
        this->grab(WORD);
    }
    return (index);
}

int Parser::parseClientBufferSize()
{
    this->grab(WORD); // client max size body
    int client_max_size_body;
    if (current_token.type == WORD)
    {
        client_max_size_body = stoi(current_token.value);
        this->grab(WORD);
    }
    else
    {
        std::cout << "Error: expected ip but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (client_max_size_body);
}

std::vector<std::string> Parser::parseAllowMethods()
{
    std::vector<std::string> allow_methods;
    this->grab(WORD); // allow_methods
    while (current_token.type == WORD)
    {
        // std::cout << "---> " << current_token.value << std::endl;
        allow_methods.push_back(current_token.value);
        this->grab(WORD);
    }
    return (allow_methods);
}

bool Parser::parseAutoIndex()
{
    this->grab(WORD); // client max size body
    std::string auto_index;
    if (current_token.type == WORD)
    {
        auto_index = current_token.value;
        this->grab(WORD);
    }
    else
    {
        std::cout << "Error: expected ip but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (auto_index == "on" ? true : false);
}

ServerConfig Parser::parseServer()
{
    ServerConfig server_setup;

    this->grab(OPEN_BRACKET);
    while (current_token.type != CLOSE_BRACKET && current_token.type != TOKEN_EOF && current_token.type != TOKEN_ERR)
    {
        if (!current_token.value.compare("listen"))
            server_setup.setServerIp(parseIp());
        else if (!current_token.value.compare("server_name"))
            server_setup.setServerName(parseServerName());
        else if (!current_token.value.compare("root"))
            server_setup.setRoot(parseRoot());
        else if (!current_token.value.compare("index"))
            server_setup.setIndexFile(parseIndex());
        else if (!current_token.value.compare("error_pages"))
            server_setup.setErrorPages(parseErrorPages());
        else if (!current_token.value.compare("client_max_body_size"))
            server_setup.setClientBufferSize(parseClientBufferSize());
        else if (!current_token.value.compare("allow_methods"))
            server_setup.setAllowMethods(parseAllowMethods());
        else if (!current_token.value.compare("autoindex"))
            server_setup.setAutoIndex(parseAutoIndex());
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