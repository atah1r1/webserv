/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 01:23:28 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/26 19:25:33 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache/2.2.14 (Win32)
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
Content-Length: 88
Content-Type: text/html
Connection: Closed

<html>
<body>
<h1>Hello, World!</h1>
</body>
</html>
*/

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>
#include <sstream>
#include <map>
#include <vector>
//#include "../Utils/StatusCodes.hpp"
#include "../Utils/Utils.hpp"

class Response {
private:
	int _status_code;
	std::string _version;
	std::string _status;
	std::string _body;
	std::map<std::string, std::string> _headers;

	static std::vector<std::string> _parseMetaData(const std::string& line);
	static std::pair<std::string, std::string> _parseHeader(const std::string& line);

public:
	Response( void );
	Response( const Response& rhs );
	Response& operator= ( const Response& rhs );
	~Response();

	std::string getVersion( void ) const;
	void setVersion( const std::string& version );
	std::string getStatus( void ) const;
	void setStatus( const std::string& status );
	int getStatusCode( void ) const;
	void setStatusCode( int status_code );
	std::string getBody( void ) const;
	void setBody( const std::string& body );
	std::map<std::string, std::string> getHeaders( void ) const;
	std::string getHeader( const std::string& key ) const;
	void setHeaders( std::map<std::string, std::string>& headers );
	void addHeader( const std::pair<std::string, std::string>& header );
	void addHeader( const std::string& key, const std::string& value );
	void removeHeader( const std::pair<std::string, std::string>& header );
	void clearAll( void );
	std::string toString( void );

	static Response parseFrom(const std::string& response);

};

#endif