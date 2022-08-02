#include "Utils.hpp"

void printRequest(Request request)
{
	std::cout << "-------------- Print Request Object --------------" << std::endl;
	std::cout << "Method: " << request.getMethod() << std::endl;
	std::cout << "Path: " << request.getPath() << std::endl;
	std::cout << "Version: " << request.getVersion() << std::endl;
	std::cout << "Host: " << request.getHost() << std::endl;
	std::cout << "Port: " << request.getPort() << std::endl;
	std::cout << "ParseState: " << request.getState() << std::endl;
	std::cout << "Headers: " << std::endl;
	std::map<std::string, std::string> headers = request.getHeaders();
	std::map<std::string, std::string>::iterator it = headers.begin();
	while (it != headers.end())
	{
		std::cout << it->first << ":" << it->second << std::endl;
		it++;
	}
	std::cout << "---------------------- End -----------------------" << std::endl;
}

void parseFirstLine(std::string line, Request *request)
{
	std::istringstream iss(line);
	std::string token;
	int i = 0;
	while (std::getline(iss, token, ' '))
	{
		if (i == 0)
			request->setMethod(token);
		else if (i == 1)
			request->setPath(token);
		else if (i == 2)
			request->setVersion(token);
		i++;
	}
	request->setState(Request::BEFORE_HEADERS);
}

void parseSecondLine(std::string line, Request *request)
{
	request->setState(Request::HEADERS);
	std::istringstream iss(line);
	std::string token;
	int i = 0;
	while (std::getline(iss, token, ':'))
	{
		if (i == 1)
			request->setHost(token.substr(1, token.size())); // remove first char ' '
		else if (i == 2)
			request->setPort(std::stoi(token));
		i++;
	}
}

void parseHeaders(std::string line, Request *request)
{
	std::istringstream iss(line);
	std::string token;
	std::string key;
	std::string value;
	int j = 0;
	while (std::getline(iss, token, ':'))
	{
		if (j == 0)
			key = token;
		else if (j == 1)
			value = token.substr(1, token.size()); // remove first char ' '
		j++;
	}
	request->setHeader(key, value);
}

void parseRequest(Request request, std::string buffer)
{
	std::istringstream is(buffer);
	std::string line;
	int i = 0;

	while (std::getline(is, line))
	{
		if (i == 0 && request.getState() == Request::FIRST_LINE)
			parseFirstLine(line, &request);
		else if (i == 1 && request.getState() == Request::BEFORE_HEADERS)
			parseSecondLine(line, &request);
		else if (request.getState() == Request::HEADERS)
		{
			if (line == "\r")
			{
				request.setState(Request::BEFORE_BODY);
				if (request.getMethod() == "GET" || request.getMethod() == "DELETE")
				{
					request.setState(Request::COMPLETED);
					break;
				}
				continue;
			}
			parseHeaders(line, &request);
		}

		else if (request.getState() == Request::BEFORE_BODY || request.getState() == Request::BODY)
		{
			std::cout << "Body: " << line << std::endl;
		}
		i++;
	}

	printRequest(request);
}

