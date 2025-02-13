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

NAME		= webserv

CPP			= c++
CPP_FLAGS	= -Wall -Wextra -Werror -std=c++98 #-Wconversion -Wunused

INCLUDE		= -I./include/						#Directiva de los headers
HEADERS		= ACommonConfigs.hpp AServerBlockConfigs.hpp Config.hpp \
			Location.hpp Server.hpp Status.hpp Utils.hpp 
HEADERS		:= $(addprefix ./include/, $(HEADERS))

SRCS		= ./templates/mainTestConfig.cpp \
			./src/ACommonConfigs.cpp \
			./src/AServerBlockConfigs.cpp \
			./src/Config.cpp \
			./src/Location.cpp \
			./src/Server.cpp \
			./src/Status.cpp \
			./src/Utils.cpp

#Object files
DIR_OBJS	= ./build/
OBJS		= $(addprefix $(DIR_OBJS), $(notdir $(patsubst %.cpp, %.o, $(SRCS))))

#Directories
DIR_SRC			= ./src/
DIR_TEMPLATES	= ./templates/

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP) $(CPP_FLAGS) $(OBJS) -o $(NAME)

# Creación de directorio objectos si no existen
$(DIR_OBJS):
	mkdir $(DIR_OBJS)

#.cpp en carpeta TEMPLATES
$(DIR_OBJS)%.o: $(DIR_TEMPLATES)%.cpp $(HEADERS) Makefile | $(DIR_OBJS)
	$(CPP) -c $(CPP_FLAGS) $(INCLUDE) $< -o $@

#.cpp en carpeta SRC
$(DIR_OBJS)%.o: $(DIR_SRC)%.cpp $(HEADERS) Makefile | $(DIR_OBJS)
	$(CPP) -c $(CPP_FLAGS) $(INCLUDE) $< -o $@


clean:
	rm -rf $(DIR_OBJS)
	
fclean: clean
	rm -rf $(NAME)

re: fclean all

do:
	@$(MAKE) > /dev/null && ./$(NAME);

profile: fclean
	@echo "Profiling result available in profile.txt"
	@$(MAKE) CPP_FLAGS="$(CPP_FLAGS) -pg" > /dev/null ;
	@./$(NAME) 2>&1 > /dev/null;
	@gprof -b $(NAME) > profile.txt;
	@rm -rf gmon.out;

valgrind:
	@$(MAKE) > /dev/null && valgrind ./$(NAME);

.PHONY: all clean fclean re do profile valgrind

