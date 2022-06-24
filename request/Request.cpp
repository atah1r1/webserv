/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:57:03 by aes-salm          #+#    #+#             */
/*   Updated: 2022/06/24 10:38:04 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

// Request::Request(/* args */)
// {
// }

// Request::~Request()
// {
// }

// GETTERS
std::string Request::getMethod()
{
	return _method;
}
std::string Request::getPath()
{
	return _path;
}
std::string Request::getVersion()
{
	return _version;
}
std::string Request::getHost()
{
	return _host;
}
int Request::getPort()
{
	return _port;
}

bool Request::isCompleted()
{
	return _completed;
}