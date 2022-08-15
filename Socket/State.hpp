/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: atahiri <atahiri@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:48:08 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/15 17:37:53 by atahiri          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

enum State {
    // normal state
    INIT = 0,
    ACCEPTED = 1,
    READ_REQUEST = 2,
    WROTE_HEADERS = 3,
    WROTE_BODY = 4,
    COMPLETED = 5,
    CLOSED = 6,

    // error state
    RECV_ERROR = -1,
    SEND_ERROR = -2,
    ERROR = -3
};
