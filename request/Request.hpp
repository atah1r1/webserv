/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/23 15:55:34 by aes-salm          #+#    #+#             */
/*   Updated: 2022/07/19 15:31:30 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <map>

class Request
{
private:
	bool _completed;
	std::string _method;
	std::string _path;
	std::string _version;
	std::string _host;
	int _port;

	std::map<std::string, std::string> _headers;

public:
	Request(/* args */);
	~Request();

	// GETTERS
	std::string getMethod();
	std::string getPath();
	std::string getVersion();
	std::string getHost();
	int getPort();
	bool isCompleted();
};

#endif