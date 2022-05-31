/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:08:24 by atahiri           #+#    #+#             */
/*   Updated: 2022/05/31 17:58:19 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig(/* args */)
{
}

ServerConfig::~ServerConfig()
{
}

std::string ServerConfig::getServerName(void) const
{
    return this->_name;
}

void ServerConfig::setServerName(std::string _name)
{
    this->_name = _name;
}

std::string ServerConfig::getServerIp(void) const
{
    return this->_ip;
}

void ServerConfig::setServerIp(std::string _ip)
{
    this->_ip = _ip;
}