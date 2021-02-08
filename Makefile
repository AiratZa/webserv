NAME = webserv

FLAGS = -std=c++98 -Wall -Wextra -Werror -g

UTILS_SRCS = utils/utils.cpp \
                utils/cpp_libft/stl_containers_operator_overloading.cpp \
                utils/get_next_line/get_next_line.cpp \
                utils/get_next_line/get_next_line_utils.cpp \
                utils/cpp_libft/get_next_word.cpp \
                utils/cpp_libft/isspace.cpp


SRCS = $(UTILS_SRCS) main.cpp WebServ.cpp Server.cpp\
    config_parsing/Config.cpp

OBJS = $(SRCS:.cpp=.o)

MAKE_DEPS = $(SRCS:.cpp=.d)

all: $(NAME)

$(NAME): $(OBJS)
	clang++ $(FLAGS) $(OBJS) -I./ -o $(NAME)

clean:
	rm -rf $(OBJS) $(MAKE_DEPS)

fclean: clean
	rm -rf $(NAME)

re:	fclean $(NAME)

%.o: %.cpp
	clang++ $(FLAGS) -c $< -o $@  -MD

include $(wildcard $(MAKE_DEPS))

.PHONY: all clean fclean re
