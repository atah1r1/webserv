/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:57:03 by aes-salm          #+#    #+#             */
/*   Updated: 2022/06/23 16:14:01 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

Request::Request(/* args */)
{
}

Request::~Request()
{
}

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
