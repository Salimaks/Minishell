# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: skassimi <skassimi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/29 09:42:38 by skassimi          #+#    #+#              #
#    Updated: 2024/11/29 09:47:40 by skassimi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
LIBFT	= libft
SOURCES = srcs/


CC = cc
CFLAGS = -Wall -Wextra -Werror -g3

SRCS_FILES =	main \
				signals \ 
SRCS 		= $(addprefix $(SOURCES), $(addsuffix .c, $(SRCS_FILES)))
OBJS		= $(SRCS:.c=.o)

.c.o:
	$(CC) $(CFLAGS) -I $(SOURCES) -c $< -o $(<:.c=.o)

all :		$(NAME)

$(NAME):	$(OBJS)
				make -C $(FT_PRINTF)
				mv	libft/libft.a .
				$(CC) $(OBJS) libft.a -o $(NAME)

clean:
	rm -f $(NAME)
	rm -f $(OBJS)
	make clean -C $(FT_PRINTF)

	
fclean: clean
	rm -f $(NAME) $(OBJS) libft.a

re: fclean all

.PHONY: all clean fclean re