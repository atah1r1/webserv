NAME 	= 	webserv
CC		= 	c++
FLAGS	= 	-Wall -Wextra -Werror -std=c++98
SRCS	= 	main.cpp\
			Config/Config.cpp\
			Config/ServerConfig.cpp\
			Config/Token.cpp\
			Config/Lexer.cpp\
			Config/Parser.cpp\
			Socket/Socket.cpp\
			Socket/Server.cpp\
			Request/Request.cpp\
      		Request/Utils.cpp\
			Utils/FileHandler.cpp\
			Utils/Utils.cpp\
			Utils/Debug.cpp\
			Response/ResponseHandler.cpp\
			Response/Response.cpp\



$(NAME) : 	$(SRCS)
			@$(CC) $(FLAGS) $(SRCS) -o $(NAME) -D ALLOW_DEBUG -O2

all		: 	$(NAME)

clean	:
			@rm -rf *.o

fclean	: 	clean
			@rm -rf $(NAME)
			@rm -rf *.dSYM/

re		: 	fclean all