/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/15 17:01:23 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Request.hpp"

void parseFirstLine(std::string line, Request *request);
void parseHeaders(std::string line, Request *request);
void parseBody(std::string line, Request *request);
void parseRequest(Request &request, const std::string& buffer);

#endif