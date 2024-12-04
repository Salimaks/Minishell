# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: alex <alex@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/14 14:56:12 by mkling            #+#    #+#              #
#    Updated: 2024/12/03 21:17:54 by alex             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

DIR_SRC		= source
DIR_EXEC	= execution
DIR_PARS	= parsing

DIR_OBJ		= object

DIR_INC		= include

DIR_LIB		= $(DIR_INC)/libft

HEADER		= $(DIR_INC)/minishell.h

FUNC_EXEC	= 	execution.c \
				readability.c \
				redirection.c \
				path.c \
				setup.c \
				errors.c

FUNC_PARS	=	readline.c \
				token.c \
				scanner.c \
				lexer.c \
				parser.c \
				signals.c \
				cleanup.c

FUNC		= 	$(addprefix $(DIR_EXEC)/, $(FUNC_EXEC)) \
				$(addprefix $(DIR_PARS)/, $(FUNC_PARS)) \

MAIN		= main.c

SRC			= $(FUNC) $(MAIN)

OBJ			= $(SRC:%.c=/%.o)

LIB			= -L$(DIR_LIB) -lft -lreadline

INC			= -I$(DIR_INC) -I$(DIR_LIB)

CC			= cc

CFLAGS		= -Wall -Wextra -Werror


#############################
#	Unit test variables		#
#############################

T_NAME		= utest

T_DIR		= test

T_SRC		= utest.c

T_OBJ		= utest.o

T_INC		= 	-I$(HOME)/Criterion/include/criterion \
				-I$(HOME)/Criterion/ \
				$(INC)

T_LIB		= 	-Wl,-rpath=$(HOME)/Criterion/build/src \
				-L$(HOME)/Criterion/build/src \
				-lcriterion \

T_CC		= cc $(CFLAGS) $(T_INC) $(T_LIB) $(LIB) -g

T_EXCL		= ./obj/main.o


#############################
#							#
#		COMPILATION			#
#							#
#############################

all:				$(NAME)

$(NAME):			$(HEADER) $(LIB)
					$(CC) $(CFLAGS) $(INC) -o $(NAME) $(addprefix $(DIR_SRC)/, $(SRC)) $(LIB)

$(OBJ_DIR)/%.o:		$(DIR_SRC)/%.c $(DIR_OBJ)
					@echo "Compiling object files $@"
					$(CC) $(CFLAGS) $(INC) -c $< -o  $(addprefix $(DIR_OBJ), $@)

$(DIR_OBJ):
					mkdir -p $(DIR_OBJ)
					mkdir -p $(DIR_OBJ)/$(DIR_EXEC)
					mkdir -p $(DIR_OBJ)/$(DIR_PARS)

$(LIB):
					make -C $(DIR_LIB)

#############################
#							#
#		TEST AND DEBUG		#
#							#
#############################

debug:		$(OBJ) $(HEADER)
			@echo "Compiling with debug flag"
			$(CC) $(CFLAGS) -g $(INC) -o $(NAME) $(addprefix $(DIR_SRC)/, $(SRC)) $(LIB)

$(T_NAME):	$(OBJ)
			@echo "Compiling unit test"
			$(T_CC) $(filter-out $(DIR_OBJ)/$(T_EXCL), $(addprefix $(DIR_OBJ), $(OBJ))) \
			$(addprefix $(T_DIR)/, $(T_SRC)) -o $(T_DIR)/$(T_NAME) $(LIB)

test:		$(T_NAME)
			@echo "Running unit tests :"
			@$(T_DIR)/$(T_NAME)

#############################
#							#
#		CLEAN UP			#
#							#
#############################

clean:
			rm -rf $(DIR_OBJ)
			rm -rf $(T_DIR)/$(T_NAME)
			make -C $(DIR_LIB) clean

fclean:
			rm -rf $(DIR_OBJ)
			rm -rf $(T_DIR)/$(T_NAME)
			rm -rf $(NAME)
			make -C $(DIR_LIB) fclean

re:			fclean all

.PHONY:		all clean fclean re debug test compile
