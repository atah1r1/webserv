/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 03:00:15 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/26 18:50:48 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void ) {
	this->_file = NULL;
	this->_version = "";
	this->_status = "";
	this->_status_code = 0;
	this->_body = "";
	this->_file_path = "";
	this->_is_buffered = false;
	this->is_from_cgi = false;
	this->_are_headers_sent = false;
}

Response::Response( const Response& rhs ) : _status_code(0), _is_buffered(false), is_from_cgi(false), _are_headers_sent(false), _file(NULL) {
	*this = rhs;
}

Response& Response::operator= ( const Response& rhs )  {
	this->_version = rhs.getVersion();
	this->_status = rhs.getStatus();
	this->_status_code = rhs.getStatusCode();
	this->_body = rhs.getBody();
	this->_headers = rhs.getHeaders();
	this->_is_buffered = rhs.isBuffered();
	this->is_from_cgi = rhs.isFromCGI();
	this->_are_headers_sent = rhs.areHeadersSent();
	this->_file_path = rhs.getFilePath();
	if (this->_file != NULL) {
		if (this->_file->is_open())
			this->_file->close();
		delete this->_file;
	}
	this->_file = rhs.getFile();
	return *this;
}

Response::~Response()  {}

std::string Response::getVersion( void ) const  {
	return this->_version;
}

void Response::setVersion( const std::string& version )  {
	this->_version = trim(version);
}

std::string Response::getStatus( void ) const  {
	return this->_status;
}

void Response::setStatus( const std::string& status )  {
	this->_status = trim(status);
}

int Response::getStatusCode( void ) const  {
	return this->_status_code;
}

void Response::setStatusCode( int status_code )  {
	this->_status_code = status_code;
}

std::string Response::getBody( void ) const  {
	return this->_body;
}

void Response::setBody( const std::string& body )  {
	this->_body = body;
}

std::vector<std::pair<std::string, std::string> > Response::getHeaders( void ) const  {
	return this->_headers;
}

std::string Response::getHeader( const std::string& key ) const {
	std::vector<std::pair<std::string, std::string> >::const_iterator it = this->_headers.begin();
	for (; it != this->_headers.end(); it++)
	{
		if (it->first == key)
			return it->second;
	}
	return "";
}

void Response::setHeaders( std::vector<std::pair<std::string, std::string> >& headers )  {
	this->_headers = headers;
}

void Response::addHeader( const std::pair<std::string, std::string>& header )  {
	if (header.first == H_SET_COOKIE) {
	_headers.push_back(header);
	} else {
		std::vector<std::pair<std::string, std::string> >::iterator it = _headers.begin();
		for (; it != _headers.end(); it++)
		{
			if (it->first == header.first)
			{
				it->second = header.second;
				return ;
			}
		}
		_headers.push_back(header);
	}
}

void Response::addHeader( const std::string& key, const std::string& value ) {
	this->addHeader(std::make_pair(toHeaderCase(trim(key)), trim(value)));
}

void Response::removeHeader( const std::string& key )  {
	if (this->_headers.empty()) return;
	std::vector<std::pair<std::string, std::string> >::iterator it = this->_headers.begin();
	for (; it != this->_headers.end(); it++) {
		if (it->first == key) {
			this->_headers.erase(it);
			return;
		}
	}
}

bool Response::isBuffered( void ) const {
	return this->_is_buffered;
}

void Response::setBuffered( bool isBuffered ) {
	this->_is_buffered = isBuffered;
}

bool Response::isFromCGI( void ) const {
	return this->is_from_cgi;
}

void Response::setFromCGI( bool isFromCGI ) {
	this->is_from_cgi = isFromCGI;
}

bool Response::areHeadersSent() const {
	return this->_are_headers_sent;
}

void Response::setHeadersSent( bool sent ) {
	this->_are_headers_sent = sent;
}

std::fstream* Response::getFile( void ) const {
	return this->_file;
}

bool Response::setupFile( void ) {
	if (this->_file == NULL)
		this->_file = new std::fstream();
	if (this->_file_path.empty()) {
		debugPrint(_ERROR, __FILE__, __LINE__, "Response: setupFile failure: _filePath is empty");
		return false;
	}

	this->_file->open( this->_file_path, std::fstream::in | std::fstream::binary );
	if (!this->_file->is_open()) {
		debugPrint(_ERROR, __FILE__, __LINE__, "Response: open failure: ");
		return false;
	}
	return true;
}

void Response::setFilePath( const std::string& path ) {
	this->_file_path = path;
}

std::string Response::getFilePath( void ) const {
	return this->_file_path;
}

size_t Response::getNextBuffer(char *buffer) {
	if (!this->_file->good())
		return 0;
	this->_file->read(buffer, BUFFER_SIZE);
	return this->_file->gcount();
}

void Response::clearAll( void )  {
	// reset file
	if (this->_file != NULL && this->_file->is_open())
		this->_file->close();
	if (this->_file != NULL)
		delete this->_file;
	this->_file = NULL;

	// unlink cgi file
	if (this->is_from_cgi && !this->_file_path.empty())
		remove(this->_file_path.c_str());

	// reset everything else
	this->_version = "";
	this->_status = "";
	this->_status_code = 0;
	this->_body = "";
	this->_file_path = "";
	this->_headers.clear();
	this->_is_buffered = false;
	this->is_from_cgi = false;
	this->_are_headers_sent = false;
}

std::string Response::toStr( void ) {
	std::stringstream results;

	// append metadata (http-version status-code status-message)
	results << this->_version << " " << this->_status_code << " " << this->_status << CRLF;

	// append headers
	std::vector<std::pair<std::string, std::string> >::iterator it = this->_headers.begin();
	while (it != this->_headers.end()) {
		results << it->first << ": " << it->second << CRLF;
		++it;
	}
	// append empty line after headers
	results << CRLF;

	// append body
	results << this->_body;
	return results.str();
}

std::pair<std::string, std::string> Response::_parseHeader( const std::string& line )  {
	std::string _key, _value;
	size_t _end = 0;

	std::string _line = trim(line);
	// key
	while (_end < _line.length() && _line[_end] != ':')	++_end;
	_key =  trim(_line.substr(0, _end));

	// value
	if (_end < _line.length())
		_value = trim(_line.substr(++_end));

	return std::make_pair(_key, _value);
}

std::pair<bool, Response> Response::parseFrom( const std::string& filePath ) {
	Response r;
	std::string _line;
	size_t _seek = 0;

	std::ifstream ifile(filePath.c_str(), std::ifstream::in | std::ifstream::binary);

	if (!ifile.is_open())
		return std::make_pair(false, r);

	while (std::getline(ifile, _line)) {
		_seek += _line.length() + 1;
		_line = trim(_line);
		if (_line.empty()) break;
		std::pair<std::string, std::string> _header = _parseHeader(_line);
		r.addHeader(_header);
	}

	r.setVersion(HTTP_VERSION);
	r.setStatusCode(OK);
	r.setStatus(getReason(OK));
	std::string _status = r.getHeader(H_STATUS);
	if (!_status.empty()) {
		r.setStatusCode(toNumber<int>(trim(_status)));
		r.setStatus(getReason(r.getStatusCode()));
	}
	r.removeHeader(H_STATUS);
	r.removeHeader(H_X_POWERED_BY);

	if (ifile.eof()) {
		ifile.close();
		remove(filePath.c_str());
		r.addHeader(H_CONTENT_LENGTH, "0");
		return std::make_pair(true, r);
	}

	ifile.close();
	r.setFilePath(filePath);
	size_t _contentLength = !r.getFilePath().empty() ? FileHandler::getFileSize(r.getFilePath()) : r.getBody().size();
	r.addHeader(H_CONTENT_LENGTH, toString<size_t>(_contentLength > 0 ? (_contentLength - _seek) : 0));
	if (!r.setupFile())
		return std::make_pair(false, r);
	r.getFile()->seekg(_seek, std::fstream::beg);
	r.setBuffered(true);
	r.setFromCGI(true);
	return std::make_pair(true, r);
}

