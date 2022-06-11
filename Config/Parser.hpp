/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/06 09:38:10 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/11 14:27:13 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_HPP
#define PARSER_HPP

#include "Lexer.hpp"
#include "ServerConfig.hpp"

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
    std::vector<ServerConfig> parse();
    ServerConfig parseServer();
    ServerConfig checkConfig(ServerConfig server_setup);
    std::string parseIp();
    std::string parseServerName();
    std::string parseRoot();
    std::vector<std::string> parseIndex();
    std::vector<std::string> parseErrorPages();
    int parseClientBufferSize();
    std::vector<std::string> parseAllowMethods();
    bool parseAutoIndex();
    Location * parseLocations();
    std::string parseRedirection();
};



#endif