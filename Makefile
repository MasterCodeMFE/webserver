# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pabad-ap <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/13 17:29:06 by pabad-ap          #+#    #+#              #
#    Updated: 2025/02/13 17:29:09 by pabad-ap         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98 #-Wconversion -Wunused

INCLUDE		= -I./include/						#Directiva de los headers
HEADERS		= main.hpp Location.hpp Server.hpp Status.hpp Utils.hpp Parser.hpp test.hpp
HEADERS		:= $(addprefix ./include/, $(HEADERS))


SRC = ./src/Location.cpp \
	./src/Server.cpp \
	./src/Status.cpp \
	./src/Utils.cpp \
	./src/parser/Parser.cpp \
	./src/parser/ParserExceptions.cpp \
    ./src/mainTestConfig.cpp \
    ./src/1serverSocketInit.cpp \
    ./src/2serverBind.cpp \
    ./src/3socketPolling.cpp \
    ./src/4connectionHandling.cpp \
    ./src/5clientRequestHandler.cpp \
    ./src/6httpRequestDispatcher.cpp \
    ./src/cgi.cpp \
    ./src/delete.cpp \
    ./src/get.cpp \
    ./src/post.cpp \
    ./src/fileManager.cpp \
    ./src/httpUtils.cpp

#Object files
OBJ_DIR	= ./build/
OBJ		= $(addprefix $(OBJ_DIR), $(notdir $(patsubst %.cpp, %.o, $(SRC))))

##Directories
DIR_SRC		= ./src/
DIR_PARSER		= $(DIR_SRC)parser/

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

#.cpp en carpeta SRC
$(OBJ_DIR)%.o: $(DIR_SRC)%.cpp $(HEADERS) Makefile | $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(INCLUDE) $< -o $@

#.cpp en carpeta PARSER
$(OBJ_DIR)%.o: $(DIR_PARSER)%.cpp $(HEADERS) Makefile | $(OBJ_DIR)
	$(CC) -c $(FLAGS) $(INCLUDE) $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

do:
	@$(MAKE) > /dev/null && ./$(NAME);

profile: fclean
	@echo "Profiling result available in profile.txt"
	@$(MAKE) FLAGS="$(FLAGS) -pg" > /dev/null ;
	@./$(NAME) 2>&1 > /dev/null;
	@gprof -b $(NAME) > profile.txt;
	@rm -rf gmon.out;

valgrind:
	@$(MAKE) > /dev/null && valgrind ./$(NAME);

.PHONY: all clean fclean re do profile valgrind

