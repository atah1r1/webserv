/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 03:00:15 by ehakam            #+#    #+#             */
/*   Updated: 2022/08/07 19:03:09 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void ) {
	this->_file = new std::fstream();
	// TODO: move this to some new function
	// _file.open(_filePath.c_str(), std::fstream::in);

	// char *buffer = {0};

	// if (_file.is_open()) {

	// 	while(!_file.eof())
	// 		_file.read(buffer, BUFFER_SIZE);
	// }
}

Response::Response( const Response& rhs )  {
	*this = rhs;
}

Response& Response::operator= ( const Response& rhs )  {
	this->_version = rhs.getVersion();
	this->_status = rhs.getStatus();
	this->_status_code = rhs.getStatusCode();
	this->_body = rhs.getBody();
	this->_headers = rhs.getHeaders();
	this->_isChunked = rhs.isChunked();
	this->_isFromCGI = rhs.isFromCGI();
	this->_file = rhs.getFile();
	return *this;
}

Response::~Response()  {
	clearAll();
}

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

bool Response::isChunked( void ) const {
	return this->_isChunked;
}

void Response::setChunked( bool isChunked ) {
	this->_isChunked = isChunked;
}

bool Response::isFromCGI( void ) const {
	return this->_isFromCGI;
}

void Response::setFromCGI( bool isFromCGI ) {
	this->_isFromCGI = isFromCGI;
}

std::fstream* Response::getFile( void ) const {
	return this->_file;
}

bool Response::setupFile( void ) {
	if (this->_file == NULL)
		this->_file = new std::fstream();
	if (this->_filePath.empty()) {
		debugPrint(_ERROR, __FILE__, __LINE__, "Response: setupFile failure: _filePath is empty");
		return false;
	}
	this->_file->open( this->_filePath, std::fstream::in | std::fstream::binary );
	if (!this->_file->is_open()) {
		debugPrint(_ERROR, __FILE__, __LINE__, "Response: open failure: ");
		return false;
	}
}

void Response::setFilePath( const std::string& path ) {
	this->_filePath = path;
}

std::string Response::getFilePath( void ) const {
	return this->_filePath;
}

bool Response::getNextChunk(char *buffer) {
	if (!this->_file->good())
		return false;
	this->_file->read(buffer, BUFFER_SIZE);
	return true;
}

void Response::clearAll( void )  {
	// reset file
	if (this->_file->is_open())
		this->_file->close();
	if (this->_file != NULL)
		delete this->_file;
	this->_file = NULL;

	// unlink cgi file
	if (this->_isFromCGI && !this->_filePath.empty())
		remove(this->_filePath.c_str());

	// reset everything else
	this->_version = "";
	this->_status = "";
	this->_status_code = 0;
	this->_body = "";
	this->_filePath = "";
	this->_headers.clear();
	this->_isChunked = false;
	this->_isFromCGI = false;
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
	results << this->_body << CRLF;
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

