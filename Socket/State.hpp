/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   State.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/15 15:48:08 by atahiri           #+#    #+#             */
/*   Updated: 2022/08/18 16:01:39 by ehakam           ###   ########.fr       */
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
    KEEP_ALIVE = 7,

    // error state
    ERROR = -1
};
