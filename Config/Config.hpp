/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:25 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/16 15:46:55 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

#include <iostream>
#include <vector>
#include "ServerConfig.hpp"

class Config: public ServerConfig
{
private:
    std::vector<ServerConfig> _servers;
public:
    Config(/* args */);
    Config(std::string file_name);
    ~Config();
    void checkErrors(std::vector<ServerConfig> config);
};

#endif