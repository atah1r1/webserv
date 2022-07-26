/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/26 03:00:15 by ehakam            #+#    #+#             */
/*   Updated: 2022/07/26 05:08:23 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response( void ) { }

Response::Response( const Response& rhs )  {
	*this = rhs;
}

Response& Response::operator= ( const Response& rhs )  {
	this->_version = rhs.getVersion();
	this->_status = rhs.getStatus();
	this->_status_code = rhs.getStatusCode();
	this->_body = rhs.getBody();
	this->_headers = rhs.getHeaders();
	return *this;
}

Response::~Response()  { }

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

void Response::setHeaders( std::map<std::string, std::string>& headers )  {
	this->_headers = headers;
}

void Response::addHeader( const std::pair<std::string, std::string>& header )  {
	this->_headers.insert(header);
}

void Response::addHeader( const std::string& key, const std::string& value ) {
	this->_headers.insert(std::make_pair(trim(key), trim(value)));
}

void Response::removeHeader( const std::pair<std::string, std::string>& header )  {
	if (this->_headers.empty()) return;
	this->_headers.erase(header.first);
}

void Response::clearAll( void )  {
	this->_version = "";
	this->_status = "";
	this->_status_code = 0;
	this->_body = "";
	this->_headers.clear();
}

std::string Response::toString( void ) {
	std::stringstream results;

	// append metadata (http-version status-code status-message)
	results << this->_version << " " << this->_status_code << " " << this->_status << "\n";

	// append headers
	std::map<std::string, std::string>::iterator it = this->_headers.begin();
	while (it != this->_headers.end()) {
		results << it->first << ": " << it->second << "\n";
		++it;
	}
	// append empty line after headers
	results << "\n";

	// append body
	results << this->_body << "\n";
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

	// metadata
	_ret = nextLine(_resp, _beg);
	_beg = _ret.first;
	_line = _ret.second;
	std::vector<std::string> _meta = _parseMetaData(_line);
	r.setVersion(_meta[0]);
	r.setStatusCode(toNumber<int>(_meta[1]));
	r.setStatus(_meta[2]);

	// headers
	while (_beg < _resp.length()) {
		_ret = nextLine(_resp, _beg);
		_beg = _ret.first;
		_line = _ret.second;
		if (_beg >= _resp.length() || _line.empty()) break;
		std::pair<std::string, std::string> _h = _parseHeader(_line);
		r.addHeader(_h);
	}

	// body
	if (_beg < _resp.length()) {
		r.setBody(trim(_resp.substr(_beg)));
	}

	return r;
}
