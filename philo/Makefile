# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/16 08:32:10 by marvin            #+#    #+#              #
#    Updated: 2025/02/16 08:32:10 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

NAME = philo

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

OUTDIR = ..

OUTNAME = $(OUTDIR)/$(NAME)

SRCS =	philo.c  \
		philo_utils.c \
		philo_utils2.c

OBJ = $(SRCS:.c=.o)

all: $(OUTNAME)

$(OUTNAME): $(SRCS) philo.h | $(OUTDIR)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	rm -f *.o

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re