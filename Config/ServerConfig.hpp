/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:08:26 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/01 15:12:54 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <list>
#include <vector>

struct Location {
    std::string _location;
    std::list<std::string> _allow_methods;   
    bool _autoindex;
    std::string _index_file;
    std::string _client_buffer_size;
};

class ServerConfig
{
private:
    std::string _name;
    std::string _ip;
    int _port;
    std::string _root;
    std::list<std::string> _error_pages;
    std::list<std::string> _allow_methods;
    std::string _client_buffer_size;
    bool _autoindex;
    std::vector<Location *> _locations;
    
    
public:
    ServerConfig(/* args */);
    ~ServerConfig();
    std::string getServerName(void) const;
    void setServerName(std::string _name);
    std::string getServerIp() const;
    void setServerIp(std::string _ip);
    int getPort() const;
    void setPort(int _ip);
    std::string getRoot() const;
    void setRoot(std::string _root);
    std::list<std::string> getErrorPages() const;
    void setErrorPages(std::list<std::string> _error_pages);
    std::list<std::string> getAllowMethods() const;
    void setAllowMethods(std::list<std::string> _allow_methods);
    std::string getClientBufferSize() const;
    void setClientBufferSize(std::string _client_buffer_size);
    bool getAutoIndex() const;
    void setAutoIndex(bool _autoindex);
    std::vector<Location *> getLocations();
    
};
