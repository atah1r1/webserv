/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/08/19 00:11:42 by ehakam           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Request.hpp"

void parseFirstLine(std::string line, Request *request);
void parseHeaders(std::string line, Request *request);
void parseBody(std::string line, Request *request);
int parseRequest(Request &request, const std::string &buffer);
void printRequest(const Request &request);

#endif