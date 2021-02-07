# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gdrake <gdrake@student.21-school.ru>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/01 23:09:24 by gdrake            #+#    #+#              #
#    Updated: 2020/05/09 19:25:11 by gdrake           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

BASE_FUNCS = ft_memset ft_bzero ft_memcpy ft_memccpy\
ft_memmove ft_memchr ft_memcmp ft_strlen\
ft_strlcpy ft_strlcat ft_strchr ft_strrchr\
ft_strnstr ft_strncmp ft_atoi ft_isalpha\
ft_isdigit ft_isalnum ft_isascii ft_isprint\
ft_toupper ft_tolower ft_calloc ft_strdup\
ft_substr ft_strjoin ft_strtrim ft_split\
ft_itoa ft_strmapi ft_putchar_fd ft_putstr_fd\
ft_putendl_fd ft_putnbr_fd

BONUS_FUNCS = ft_lstnew ft_lstadd_front ft_lstsize\
ft_lstlast ft_lstadd_back ft_lstdelone\
ft_lstclear ft_lstiter ft_lstmap

BASE_SRCS = $(addsuffix .c, $(BASE_FUNCS))

BASE_OBJS = $(addsuffix .o, $(BASE_FUNCS))

BONUS_SRCS = $(addsuffix .c, $(BONUS_FUNCS))

BONUS_OBJS = $(addsuffix .o, $(BONUS_FUNCS))

FLAGS = -Wall -Wextra -Werror

INCLUDES = ./

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(BASE_OBJS)
	@ar rc $(NAME) $(BASE_OBJS)
	@ranlib $(NAME)
	@echo "======= BASE PART of Libft is created ^_^ ======="

clean:
	@rm -f $(BASE_OBJS) $(BONUS_OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

bonus: $(BASE_OBJS) $(BONUS_OBJS)
	@ar rc $(NAME) $(BASE_OBJS) $(BONUS_OBJS)
	@ranlib $(NAME)
	@echo "======= Libft with BONUS PART is created ^_^ ======="

%.o: %.c
	@gcc $(FLAGS) -c $^ -o $@ -I$(INCLUDES)
	@echo "-- $@ is compiled"
