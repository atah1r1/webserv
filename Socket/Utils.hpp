/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/20 10:42:12 by atahiri           #+#    #+#             */
/*   Updated: 2022/06/21 17:20:39 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// set fd read and fd write

void set_fds(fd_set &SocketsRead, fd_set &SocketsWrite, std::vector<int> server_fds)
{

    size_t size = server_fds.size();
    std::vector<int>::iterator it(server_fds.begin());

    FD_ZERO(&SocketsRead);
    FD_ZERO(&SocketsWrite);
    for (size_t i = 0; i < size; i++)
        FD_SET((*it++), &SocketsRead);
}

std::pair<bool, std::pair<int, size_t> > find_fd(int fd, std::vector<int> server_fds)
{
    size_t size = server_fds.size();
    std::vector<int>::iterator it(server_fds.begin());

    for (size_t i = 0; i < size; i++)
    {
        if (fd == *it)
            return std::make_pair(true, std::make_pair(*it, i));
        it++;
    }
    return std::make_pair(false, std::make_pair(0, 0));
}