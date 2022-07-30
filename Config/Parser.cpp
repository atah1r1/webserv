/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 09:38:49 by atahiri           #+#    #+#             */
/*   Updated: 2022/07/30 15:28:36 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include <iostream>

bool validateIpAddress(const std::string &ipAddress);

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
        std::cout << "Error: expected server_name but got " << this->current_token.type << std::endl;
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
        std::cout << "Error: expected root but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (root);
}

std::string Parser::parseRedirection()
{
    this->grab(WORD); // redirection
    if (current_token.type == WORD)
    {
        if (current_token.value != "301")
        {
            std::cout << "Error: Redirection must use 301" << std::endl;
            exit(1);
        }
    }
    this->grab(WORD);
    std::string redirection_path;
    if (current_token.type == WORD)
    {
        redirection_path = current_token.value;
        this->grab(WORD);
    }
    else
    {
        std::cout << "Error: expected redirect but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (redirection_path);
}

std::string Parser::parseUploadStore()
{
    this->grab(WORD); // redirection
    std::string upload_store;
    if (current_token.type == WORD)
    {
        upload_store = current_token.value;
        this->grab(WORD);
    }
    else
    {
        std::cout << "Error: expected redirect but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (upload_store);
}



std::vector<std::string> Parser::parseIndex()
{
    std::vector<std::string> index;
    this->grab(WORD); // index
    while (current_token.type == WORD)
    {
        index.push_back(current_token.value);
        this->grab(WORD);
    }
    return (index);
}

std::pair<int, std::string> Parser::parseErrorPages()
{
    std::pair<int, std::string> index;
    this->grab(WORD); // index
    if (current_token.type == WORD)
    {
        // std::cout << current_token.value << " INSIDE ERROR PAGES WHILE" << std::endl;
        int code = std::stoi(current_token.value);
        this->grab(WORD);
        // std::cout <<  " AFTER GRAB" << current_token.value  << std::endl;
        // index[code] = current_token.value;
        index.first = code;
        index.second = current_token.value;
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
        std::cout << "Error: expected client max body size but got " << this->current_token.type << std::endl;
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
        // std::cout << "ETET: " << current_token.value << std::endl;
        allow_methods.push_back(current_token.value);
        this->grab(WORD);
    }
    if (!allow_methods.empty())
    {
        for (std::vector<std::string>::iterator it = allow_methods.begin(); it != allow_methods.end(); ++it)
        {
            if (*it != "DELETE" && *it != "GET" && *it != "POST")
            {
                std::cout << "Error: expected valid method but got " << *it << std::endl;
                exit(1);
            }
        }
    } else {
        std::cout << "Error: expected valid method but got " << this->current_token.type << std::endl;
        exit(1);
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
        std::cout << "Error: expected autoindex but got " << this->current_token.type << std::endl;
        exit(1);
    }
    return (auto_index == "on" ? true : false);
}

Location *Parser::parseLocations()
{
    Location *location = new Location();
    this->grab(WORD); // path
    location->_location = current_token.value;
    this->grab(WORD);
    this->grab(OPEN_BRACKET);
    while (current_token.type != CLOSE_BRACKET && current_token.type != TOKEN_EOF && current_token.type != TOKEN_ERR)
    {
        if (!current_token.value.compare("root"))
            location->_root = parseRoot();
        else if (!current_token.value.compare("allow_methods"))
            location->_allow_methods = parseAllowMethods();
        else if (!current_token.value.compare("index"))
            location->_index_file = parseIndex();
        else if (!current_token.value.compare("autoindex"))
            location->_autoindex = parseAutoIndex();
        else if (!current_token.value.compare("return"))
            location->_redirection_path = parseRedirection();
        else if (!current_token.value.compare("upload_store"))
            location->_upload_store = parseUploadStore();
        else
        {
            std::cout << "Invalid Token" << std::endl;
        }
        this->grab(SEMICOLON);
    }
    this->grab(CLOSE_BRACKET);
    return (location);
}

ServerConfig Parser::checkConfig(ServerConfig server_setup)
{
    if (server_setup.getPort() == 0)
    {
        std::cout << "Error: bad or no port founded" << std::endl;
        exit(1);
    }
    if (validateIpAddress(server_setup.getServerIp()) == 0)
    {
        std::cout << "Error: bad or no ip founded" << std::endl;
        exit(1);
    }
    if (server_setup.getServerName().empty())
    {
        std::cout << "Error: bad or no server name founded" << std::endl;
        exit(1);
    }
    if (server_setup.getPort() <= 0)
    {
        std::cout << "Error: ip address cannot be negative" << std::endl;
        exit(1);
    }
    if (server_setup.getClientBufferSize() <= 0)
    {
        std::cout << "Error: client max body size cannot be negative" << std::endl;
        exit(1);
    }
    return server_setup;
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
        else if (!current_token.value.compare("error_page"))
        {
            server_setup.setErrorPages(parseErrorPages());
        }
        else if (!current_token.value.compare("client_max_body_size"))
            server_setup.setClientBufferSize(parseClientBufferSize());
        else if (!current_token.value.compare("location"))
        {
            server_setup.setLocations(parseLocations());
            continue;
        }
        else
        {
            std::cout << "Invalid Token" << std::endl;
        }
        this->grab(SEMICOLON);
    }
    this->grab(CLOSE_BRACKET);
    return (checkConfig(server_setup));
}