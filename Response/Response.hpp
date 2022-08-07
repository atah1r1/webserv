/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 01:23:28 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/07 21:56:33 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include "../Utils/StatusCodes.hpp"
#include "../Utils/Utils.hpp"

#define BUFFER_SIZE (1024 * 16)

class Response {
private:
	int _status_code;
	std::string _version;
	std::string _status;
	std::string _body;
	bool _isChunked;
	bool _isFromCGI;
	std::fstream* _file;
	std::string _filePath;
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
	bool isChunked( void ) const;
	void setChunked( bool isChunked );
	bool isFromCGI( void ) const;
	void setFromCGI( bool isfromCGI );
	std::fstream* getFile( void ) const;
	bool setupFile( void );
	void setFilePath( const std::string& path );
	std::string getFilePath( void ) const;
	size_t getNextChunk(char *buffer);

	void clearAll( void );
	std::string toString( void );

	static Response parseFrom( const std::string& response );
};

#endif