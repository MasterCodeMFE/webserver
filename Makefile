NAME = webserv

CC = c++
FLAGS = -Wall -Wextra -Werror -std=c++98

SRC = ./src/1.cpp \
      ./src/2.cpp \
      ./src/3.cpp \
      ./src/4.cpp \
      ./src/5.cpp \
      ./src/6.cpp \
      ./templates/mainTestConfig.cpp \
      ./templates/Config.cpp \
      ./templates/Location.cpp \
      ./templates/Server.cpp \
      ./templates/Status.cpp \
      ./templates/Utils.cpp

OBJ_DIR = obj
OBJ = $(patsubst ./src/%.cpp, $(OBJ_DIR)/%.o, $(filter ./src/%.cpp, $(SRC)))
OBJ += $(patsubst ./templates/%.cpp, $(OBJ_DIR)/%.o, $(filter ./templates/%.cpp, $(SRC)))

all: $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: ./src/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: ./templates/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(FLAGS) -o $(NAME) $(OBJ)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all
