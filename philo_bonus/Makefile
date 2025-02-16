# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/16 08:32:45 by marvin            #+#    #+#              #
#    Updated: 2025/02/16 08:32:45 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

NAME = philo_bonus

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

SRCS =	philo_bonus.c  \
		philo_utils_bonus.c \

OBJ = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS) philo_bonus.h
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re