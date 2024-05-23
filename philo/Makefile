# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amdemuyn <amdemuyn@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/07 19:21:00 by amdemuyn          #+#    #+#              #
#    Updated: 2023/12/07 19:21:06 by amdemuyn         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

YELLOW =		\033[0;93m
RED =			\033[0;91m
GREEN =			\033[0;92m

NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror #-pthread -g3 -fsanitize=thread

#-pthread
#-g3
#-g3 -fsanitize=address
#-g3 -fsanitize=thread

SRC = main.c init.c actions.c utils.c

OBJ = ${SRC:.c=.o}

all: ${NAME}

${NAME}: ${OBJ}
	@echo "$(GREEN)"
		${CC} ${CFLAGS} ${OBJ} -o ${NAME}
		@echo "\n$(GREEN)✅ Objects and executables created successfully!\n"

clean:
	@echo "\n$(YELLOW)✅ Objects cleaned successfully!\n"
	rm -rf ${OBJ}

fclean: clean
	@echo "\n$(RED)✅ Objects and executables deleted successfully!\n"
	rm -rf ${NAME}
	@echo "$(GREEN)"

re: fclean all

.PHONY: all clean fclean re
