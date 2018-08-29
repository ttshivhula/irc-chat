# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/08/25 11:29:48 by ttshivhu          #+#    #+#              #
#*   Updated: 2018/08/29 15:42:26 by ttshivhu         ###   ########.fr       *#
#                                                                              #
# **************************************************************************** #

SERVER = server
CLIENT = client
CC = gcc
CFLAGS = -Wall -Werror -Wextra -I. -Ilibft
SRC = broadcast.c generic.c server_buffer.c \

OBJ = $(SRC:.c=.o)
all: $(SERVER)

$(SERVER): $(OBJ)
	@make -C libft
	gcc -c $(CFLAGS) cmain.c
	gcc -c $(CFLAGS) smain.c
	gcc $(OBJ) smain.o $(CFLAGS) -Llibft -lft -o $(SERVER)
	gcc $(OBJ) smain.o $(CFLAGS) -Llibft -lft -o $(CLIENT)
	@printf "\x1b[32mCompiled $(CLIENT) and $(SERVER)\x1b[0m\n"

clean:
	@make clean -C libft
	/bin/rm -f $(OBJ)
	/bin/rm -f cmain.o smain.o

fclean: clean
	make fclean -C libft
	/bin/rm -f $(SERVER) $(CLIENT)

re: fclean all
