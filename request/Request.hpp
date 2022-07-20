/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:55:34 by aes-salm          #+#    #+#             */
/*   Updated: 2022/06/24 15:15:45 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "../Includes/Webserv.hpp"

class Request
{
private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::string _host;
	int _port;

	std::map<std::string, std::string> _headers;

public:
	Request(/* args */){};
	~Request(){};

	// GETTERS
	std::string getMethod();
	std::string getPath();
	std::string getVersion();
	std::string getHost();
	int getPort();
};

#endif