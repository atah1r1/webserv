#ifndef __PARSER__HPP__
#define __PARSER__HPP__

#include "Lexer.hpp"
#include "ServerSetup.hpp"

#define ERR_MSG_DIR_ERR_PAGES "Bad parameter in error_pages directve"
#define ERR_MSG_CONTEXT_SERVER "Invalid Context; support only server context"
#define ERR_MSG_MONDATORY_CONFIG "Invalid Context; Config must have a listen/root directive"



class Parser
{
    public:
    Lexer   lexer;
    Token   curr_token;
    Token   prev_token;


    Parser(Lexer lexer);
    Parser(const Parser& parser);
    Parser& operator=(const Parser& parser);


    void                                        eat(TypeToken token_type);
    int                                         eatServer();
    std::vector<ServerSetup>                    parse(char ***envp);
    ServerSetup                                 parseServer();
    std::pair<short, u_int32_t>                 parseListen();
    std::vector<std::string>                    parseWords();
    std::string                                 parseWord();
    std::vector<std::pair<short, std::string> > parseErrorPages();
    t_location                                  parseLocation();
    ServerSetup                                 CheckConfig(ServerSetup &server);


    static  void errorDisplay(std::string s);

}; // class Parser

#endif