#include "Request.hpp"


void ParseRequest(Request request, std::string buffer)
{
    (void)request;
    std::cout << request.getMethod() << std::endl;
    std::cout << "BUFFER FROM REQ" << std::endl;
    std::cout << buffer << std::endl;
}