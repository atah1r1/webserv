/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:08:26 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/19 17:46:11 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <vector>
#include <map>
#include <utility>

struct Location
{
	std::string _location;
	std::vector<std::string> _allow_methods;
	bool _autoindex;
	std::vector<std::string> _index_file;
	std::string _client_buffer_size;
	std::string _root;
	std::string _redirection_path;
	std::string _upload_store;
	std::map<std::string, std::string> _cgis;
};

class ServerConfig
{
private:
	std::vector<std::string> _server_names;
	std::string _ip;
	int _port;
	std::string _root;
	std::map<int, std::string> _error_pages;
	std::vector<std::string> _allow_methods;
	std::vector<std::string> _index_file;
	int _client_buffer_size;
	bool _autoindex;
	std::vector<Location *> _locations;

public:
	ServerConfig(/* args */);
	~ServerConfig();
	std::vector<std::string> getServerNames(void) const;
	void setServerNames(std::vector<std::string> server_names);
	std::string getServerIp() const;
	void setServerIp(std::string _ip);
	int getPort() const;
	void setPort(int _ip);
	std::string getRoot() const;
	void setRoot(std::string _root);
	std::map<int, std::string> getErrorPages() const;
	std::string getErrorPage( int code );
	void setErrorPages(std::pair<int, std::string> _error_pages);
	std::vector<std::string> getAllowMethods() const;
	void setAllowMethods(std::vector<std::string> _allow_methods);
	int getClientBufferSize() const;
	void setClientBufferSize(int _client_buffer_size);
	bool getAutoIndex() const;
	void setAutoIndex(bool _autoindex);
	std::vector<Location *> getLocations();
	void setLocations(Location *_locations);
	void setIndexFile(std::vector<std::string> _index_file);
	std::vector<std::string> getIndexFiles(void);
};

#endif