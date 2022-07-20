/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aes-salm <aes-salm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/24 12:22:25 by aes-salm          #+#    #+#             */
/*   Updated: 2022/07/20 13:19:27 by aes-salm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Request.hpp"

void parseRequest(Request request, std::string buffer)
{
	(void)request;
	std::cout << request.getMethod() << std::endl;
	std::cout << "BUFFER FROM REQ" << std::endl;
	std::cout << buffer << std::endl;
}