# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehakam <ehakam@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/02 15:05:39 by ehakam            #+#    #+#              #
#    Updated: 2022/08/02 17:17:18 by ehakam           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=a.out
SRC= ../Config/*.cpp ../CGI/*.cpp ../Request/*.cpp ../Response/*.cpp  ../Socket/*.cpp ../Utils/*.cpp main1.cpp

CC=c++

all: $(NAME)

$(NAME): $(SRC)
	@$(CC) $(SRC) -o $(NAME)

clean:
	@rm -rf *.o

fclean: clean
	@rm -rf $(NAME) *.out

re: fclean all
