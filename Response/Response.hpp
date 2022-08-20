/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 01:23:28 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/20 15:27:05 by ehakam           ###   ########.fr       */
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
#include "../Utils/FileHandler.hpp"

#define BUFFER_SIZE (1024)

class Response {
private:
	int _status_code;
	std::string _version;
	std::string _status;
	std::string _body;
	bool _is_buffered;
	bool is_from_cgi;
	bool _are_headers_sent;
	std::fstream* _file;
	std::string _file_path;
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
	void removeHeader( const std::string& key );
	bool isBuffered( void ) const;
	void setBuffered( bool isBuffered );
	bool isFromCGI( void ) const;
	void setFromCGI( bool isfromCGI );
	std::fstream* getFile( void ) const;
	bool setupFile( void );
	void setFilePath( const std::string& path );
	std::string getFilePath( void ) const;
	size_t getNextBuffer(char *buffer);
	bool areHeadersSent() const;
	void setHeadersSent( bool sent );

	void clearAll( void );
	std::string toStr( void );

	static std::pair<bool, Response> parseFrom( const std::string& filePath );
};

#endif