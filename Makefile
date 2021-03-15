NAME = webserv

FLAGS = -std=c++98 -Wall -Wextra -Werror -g

UTILS_SRCS = utils/utils.cpp \
                utils/cpp_libft/stl_containers_operator_overloading.cpp \
                utils/get_next_line/get_next_line.cpp \
                utils/get_next_line/get_next_line_utils.cpp \
                utils/cpp_libft/get_next_word.cpp \
                utils/cpp_libft/isspace.cpp \
                utils/cpp_libft/isdigit.cpp \
                utils/cpp_libft/unsigned_number_len.cpp \
                utils/cpp_libft/atoi.cpp\
                utils/cpp_libft/tolower.cpp\
                utils/cpp_libft/is_correct_ip_octet.cpp \
                utils/cpp_libft/strtoul_base.cpp \
                utils/cpp_libft/string_to_lower.cpp \
                utils/cpp_libft/ultostr_base.cpp \
                utils/cpp_libft/stoll_base.cpp \
                utils/cpp_libft/percent_decode.cpp \
                utils/cpp_libft/toupper.cpp \
                utils/cpp_libft/string_to_upper.cpp

CONF_PARSE_SRCS =  config_parsing/Config.cpp \
                   config_parsing/ServerContext.cpp \
                   config_parsing/LocationContext.cpp


CORE_SRCS =     core/WebServ.cpp \
                core/WebServ_Router.cpp \
                core/Listener.cpp \
                core/Server.cpp\
                core/Request/Request.cpp \
                core/Response/Response.cpp \
                core/Response/autoindex_handling/autoindex_handling.cpp \
                base64_coding/base64.cpp

SRCS = $(UTILS_SRCS) $(CONF_PARSE_SRCS) $(CORE_SRCS) main.cpp

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
