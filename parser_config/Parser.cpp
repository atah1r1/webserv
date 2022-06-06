#include "include/Parser.hpp"
#include "../include/Utils.hpp" // StringtoInt(), isNumber
#include <arpa/inet.h>  // inet_addr

// --------------------------------------------------------- //
// --------------- Constructors and Operators -------------- //
// --------------------------------------------------------- //

Parser::Parser(Lexer lexer) : lexer(lexer)
{
    this->curr_token = this->lexer.getNextToken();
    this->prev_token = this->curr_token;
}

Parser::Parser(const Parser& parser)
{
    *this = parser;
}

Parser&    Parser::operator=(const Parser& parser)
{
    this->lexer = parser.lexer;
    this->curr_token = parser.curr_token;
    this->prev_token = parser.prev_token;
    return (*this);
}


// --------------------------------------------------------- //
// ----------------- Member Methods ------------------------ //
// --------------------------------------------------------- //

void    Parser::eat(TypeToken token_type)
{
    if (this->curr_token.type == token_type)
	{
		this->prev_token = this->curr_token;
		this->curr_token = this->lexer.getNextToken();
	}
	else
	{
		std::cerr  << "Unexepected token '" << this->curr_token.value
                << "', with type " << this->curr_token.type << std::endl;
		exit(EXIT_FAILURE);
	}
}
 
std::vector<ServerSetup>    Parser::parse(char ***envp) // error multi server
{
    std::vector<ServerSetup> servers;
    this->eatServer(); // check if is server context and advance to next token
    servers.push_back(parseServer());

    int count_server = 0;
    while (curr_token.type != TOKEN_EOF)
    {
        this->eatServer();
        servers.push_back(parseServer());
        servers[count_server++].setEnvp(envp);
    }
    return (servers);
}

ServerSetup                 Parser::parseServer()
{
    ServerSetup server_setup;

    this->eat(OPEN_BRACKET);
    while (curr_token.type != CLOSE_BRACKET && curr_token.type != TOKEN_EOF
            && curr_token.type != TOKEN_ERR)
    {
        if (!curr_token.value.compare("listen") && server_setup.listen.first == -1)
            server_setup.listen = parseListen();
        else if (!curr_token.value.compare("server_name") && server_setup.server_name.empty())
            server_setup.server_name = parseWords();
        else if (!curr_token.value.compare("root") && !server_setup.root.length())
            server_setup.root = parseWord();
        else if (!curr_token.value.compare("index") && server_setup.index.empty())
            server_setup.index = parseWords();
        else if (!curr_token.value.compare("error_pages") && server_setup.error_pages.empty())
            server_setup.error_pages = parseErrorPages();
        else if (!curr_token.value.compare("client_max_body_size") && server_setup.client_max_body_size == -1)
            server_setup.client_max_body_size = stringToInt(parseWord());
        else if (!curr_token.value.compare("request_method") && server_setup.request_method.empty())
            server_setup.request_method = parseWords();
        else if (!curr_token.value.compare("autoindex") && !server_setup.autoindex.length())
            server_setup.autoindex = parseWord();
        else if (!curr_token.value.compare("location"))
        {
            server_setup.locations.push_back(parseLocation());
            continue;
        }
        else
            errorDisplay("Invalid Token");
        this->eat(SEMICOLON);
    }
    this->eat(CLOSE_BRACKET);
    return (CheckConfig(server_setup));
}

std::pair<short, u_int32_t> Parser::parseListen()
{
   std::pair<short, u_int32_t>  listen;
   
    this->eat(WORD); // listen
    this->eat(WORD); // port
    listen.first =  stringToInt(prev_token.value);
    this->eat(WORD); // ip 
    if (prev_token.value == "0.0.0.0" || prev_token.value == "127.0.0.1")
        listen.second = inet_addr(prev_token.value.c_str());
    else
        errorDisplay(prev_token.value + ": IP address listening to is not a valid ip interface!");

    return (listen);
}

std::vector<std::string>    Parser::parseWords()
{
    std::vector<std::string>    words;

    this->eat(WORD); // direcvtive
    while (this->curr_token.type != SEMICOLON)
    {
        words.push_back(this->curr_token.value);
        this->eat(WORD);
    }
    return (words);
}


std::string                 Parser::parseWord()
{
    this->eat(WORD); // Directive
    std::string value = this->curr_token.value;
    this->eat(WORD); // Value
    return (value);
}

std::vector<std::pair<short, std::string> >   Parser::parseErrorPages()
{
    std::vector<std::pair<short, std::string> > words;
    int                                         n_error_page = 0;
    this->eat(WORD); // Directive "error_pages"
    
    while (this->curr_token.type != SEMICOLON && isNumber(curr_token.value)) 
    {
        words.push_back(std::make_pair(stringToInt(curr_token.value), std::string())); // add number error in first pair
        this->eat(WORD); // 404 ex:
    }
    if (!words.size())
        errorDisplay(ERR_MSG_DIR_ERR_PAGES);
    
    while (this->curr_token.type != SEMICOLON)
    {
        if (n_error_page > (int)words.size())
            errorDisplay(ERR_MSG_DIR_ERR_PAGES);
        words[n_error_page].second =  curr_token.value; // add path error_page in second pair
        this->eat(WORD); // error_page_505.html ex:
        n_error_page++;
    }
    if (n_error_page != (int)words.size())
        errorDisplay(ERR_MSG_DIR_ERR_PAGES);
    return (words);
}

t_location                  Parser::parseLocation()
{
    t_location location = ServerSetup::initLocation();

    location.path = parseWord(); // path of location
    this->eat(OPEN_BRACKET);
    while (curr_token.type != CLOSE_BRACKET && curr_token.type != TOKEN_EOF
            && curr_token.type != TOKEN_ERR)
    {
        if (!curr_token.value.compare("root") && !location.root.length())
            location.root = parseWord();
        else if (!curr_token.value.compare("index") && location.index.empty())
            location.index = parseWords();
        else if (!curr_token.value.compare("error_pages") && location.error_pages.empty())
            location.error_pages = parseErrorPages();
        else if (!curr_token.value.compare("client_max_body_size") && location.client_max_body_size == -1)
            location.client_max_body_size = stringToInt(parseWord());
        else if (!curr_token.value.compare("request_method") && location.request_method.empty())
            location.request_method = parseWords();
        else if (!curr_token.value.compare("autoindex") && !location.autoindex.length())
            location.autoindex = parseWord();
        else
            errorDisplay("Invalid Token");
        this->eat(SEMICOLON);
    }
    this->eat(CLOSE_BRACKET);
    return (location);
}

int                          Parser::eatServer()
{
    if (curr_token.value.compare("server")) // != "server"
        errorDisplay(ERR_MSG_CONTEXT_SERVER);
    this->eat(WORD); // server directive
    return (0);
}

ServerSetup                 Parser::CheckConfig(ServerSetup &server)
{
    if (server.getRoot().length() == 0)
        errorDisplay(ERR_MSG_MONDATORY_CONFIG);
    else if (server.getListen().first == -1)
        errorDisplay(ERR_MSG_MONDATORY_CONFIG);
    return (server);
}

//-------------------------------- Non Member Methods ----------------------- //

void Parser::errorDisplay(std::string s)
{
    std::cerr << "Error: " << s << std::endl;
    exit(EXIT_FAILURE);
}