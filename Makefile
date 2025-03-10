# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hhecquet <hhecquet@student.42perpignan.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/12 11:23:20 by marvin            #+#    #+#              #
#    Updated: 2025/03/10 15:48:35 by hhecquet         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

.SILENT:

R = \033[31m#Red
BR = \033[91m#Bright Red
O = \033[33m#Orange
Y = \033[93m#Yellow
BG = \033[92m#Bright Green
G = \033[32m#Green
BC = \033[96m#Bright Cyan
C = \033[36m#Cyan
BB = \033[94m#Bright Blue
B = \033[34m#Blue
BM = \033[95m#Bright Magenta
M = \033[35m#Magenta
RE = \033[0m#Reset
BO = \033[1m#Bold

NAME = philo

BONUS = philo_bonus

.PHONY: all bonus clean fclean re FORCE

all: $(NAME)

$(NAME): FORCE
	echo "Compiling Philosophers..."
	cd philo && make -s all
	echo "$(BO)$(NAME) built successfully! 🤓👆"
	echo "$(M)       _____"
	echo "      /      \\"
	echo "     (____/\\  )"
	echo "      |___   ?(____            sometimes i sits and thinks"
	echo "      _\\L.   |      \\     ___"
	echo "    / /^^^\\ /.-'     |   |\\  |     but usually i'm just pooping"
	echo "   ( /  _/u     |    \\___|_)_|_          "
	echo "    \\|  \\\\      /    / \\_(______)         "
	echo "     |   \\\\    /    /  |  |    |         "
	echo "     |    ) _/    /   |  |    |         "
	echo "     _\\__/       /___(   |    |         "
	echo "  _/  __________/     \\  |    |         "
	echo " //  /  (              ) |    |         "
	echo "( \\__|___\\    \\______ /__|____|         "
	echo " \\    (___\\   |______)_/                "
	echo "  \\   |\\   \\  \\     /                   "
	echo "   \\  | \\__ |  )___/                    "
	echo "    \\  \\  )/  /__(                      "
	echo "___ |  /_//   |   \\_________            "
	echo "  _/  / / Uuuuu    \\                    "
	echo " \`----'(____________)                   $(RE)"                                    

clean:
	echo "Cleaning philosophers object files..."
	cd philo && make clean -s
	rm -f *.o
	echo "Philosophers object files have been cleaned 👌"

fclean: clean
	cd philo && make fclean -s
	echo "Philosophers itself has been cleaned 🌟"

re: fclean all
