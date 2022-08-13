/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 03:00:15 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/12 02:48:58 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void ) {
	this->_file = new std::fstream();
	this->_version = "";
	this->_status = "";
	this->_status_code = 0;
	this->_body = "";
	this->_file_path = "";
	this->_is_buffered = false;
	this->is_from_cgi = false;
}

Response::Response( const Response& rhs ) : _status_code(0), _is_buffered(false), is_from_cgi(false), _file(NULL) {
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
	this->_body = trim(body);
}

std::map<std::string, std::string> Response::getHeaders( void ) const  {
	return this->_headers;
}

std::string Response::getHeader( const std::string& key ) const {
	std::map<std::string, std::string>::const_iterator it = this->_headers.find(key);
	if (it != this->_headers.end())
		return it->second;
	return "";
}

void Response::setHeaders( std::map<std::string, std::string>& headers )  {
	this->_headers = headers;
}

void Response::addHeader( const std::pair<std::string, std::string>& header )  {
	this->_headers.insert(header);
}

void Response::addHeader( const std::string& key, const std::string& value ) {
	this->addHeader(std::make_pair(toHeaderCase(trim(key)), trim(value)));
}

void Response::removeHeader( const std::pair<std::string, std::string>& header )  {
	if (this->_headers.empty()) return;
	this->_headers.erase(header.first);
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
	if (this->_file->is_open())
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
}

std::string Response::toString( void ) {
	std::stringstream results;

	// append metadata (http-version status-code status-message)
	results << this->_version << " " << this->_status_code << " " << this->_status << CRLF;

	// append headers
	std::map<std::string, std::string>::iterator it = this->_headers.begin();
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

std::vector<std::string> Response::_parseMetaData(const std::string& line) {
	std::vector<std::string> res;
	size_t _beg = 0, _end = 0;

	std::string _line = trim(line);
	// version
	while (_end < _line.length() && _line[_end] != ' ')	++_end;
	res.push_back(_line.substr(_beg, _end - _beg));
	if (_end < _line.length()) _beg = ++_end;

	// status_code
	while (_end < _line.length() && _line[_end] != ' ')	++_end;
	res.push_back(_line.substr(_beg, _end - _beg));
	if (_end < _line.length()) _beg = ++_end;

	// status
	while (_end < _line.length()) ++_end;
	res.push_back(_line.substr(_beg, _end - _beg));

	return res;
}

std::pair<std::string, std::string> Response::_parseHeader(const std::string& line)  {
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

Response Response::parseFrom(const std::string& response) {
	Response r;
	size_t _beg = 0;
	std::string _line;
	std::pair<size_t, std::string> _ret;

	const std::string _resp = trim(response);

	r.setVersion("HTTP/1.1");
	r.setStatusCode(OK);
	r.setStatus(getReason(OK));

	// headers
	while (_beg < _resp.length()) {
		_ret = nextLine(_resp, _beg);
		_beg = _ret.first;
		_line = _ret.second;
		if (_beg >= _resp.length() || _line.empty()) break;
		std::pair<std::string, std::string> _h = _parseHeader(_line);
		r.addHeader(_h);
	}

	// set status based on Status header.
	std::string code = r.getHeader("Status");
	if (!code.empty()) {
		int codeInt = toNumber<int>(code);
		r.setStatusCode(codeInt);
		r.setStatus(getReason(codeInt));
	}

	// body
	if (_beg < _resp.length()) {
		r.setBody(trim(_resp.substr(_beg)));
	}

	return r;
}

