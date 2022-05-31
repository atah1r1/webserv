/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/31 16:04:25 by atahiri           #+#    #+#             */
/*   Updated: 2022/05/31 17:27:14 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include "ServerConfig.hpp"

class Config
{
private:
    std::vector<ServerConfig *> _servers;
public:
    Config(/* args */);
    ~Config();
};
