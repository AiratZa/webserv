NAME = config_parsing_tests

GTEST_FLAGS = -lpthread -lgtest -lgtest_main -L../../google_test/include/gtest

FLAGS = -std=c++98 -Wall -Wextra -Werror -g



SRCS = main.cpp


OBJS = $(SRCS:.cpp=.o)

MAKE_DEPS = $(SRCS:.cpp=.d)

all: $(NAME)

$(NAME): $(OBJS)
	clang++ $(FLAGS) $(GTEST_FLAGS) $(OBJS) -I./ -o $(NAME)

clean:
	rm -rf $(OBJS) $(MAKE_DEPS)

fclean: clean
	rm -rf $(NAME)

re:	fclean $(NAME)

%.o: %.cpp
	clang++ $(FLAGS) -c $< -o $@  -MD

include $(wildcard $(MAKE_DEPS))

.PHONY: all clean fclean re
