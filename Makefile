# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bclaeys <bclaeys@student.s19.be>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/24 11:58:53 by bclaeys           #+#    #+#              #
#    Updated: 2024/10/24 16:30:31 by bclaeys          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=minishell

MAIN_SRCS= minishell_main.c init_data.c
CLI_SRCS =
EXECUTOR_SRCS =
LEXER_SRCS =
PARSER_SRCS =
SIGHANDLER_SRCS =

CC=cc
CFLAGS= -Wall -Wextra -Werror -g
INCLUDES = -I ./libft
INCFLAGS= -I src/libft -L src/libft -lft 
LIBFT=./src/libft/libft.a

SRCS = $(MAIN_SRCS) $(CLI_SCRS) $(EXECUTOR_SRCS) \
	$(LEXER_SRCS) $(PARSER_SRCS) $(SIGHANDLER_SRCS)

SRC_DIRS = src/cli src/executor/builtins src/executor/exec_base \
		   src/executor/pipe src/executor/redir src/lexer src/main \
		   src/parser src/sighandler
vpath %.c $(SRC_DIRS)  

OBJECTS = $(patsubst %.c,obj/%.o,$(SRCS))

all: $(LIBFT) obj $(NAME)
	@(make -q $(NAME) && echo "Everything up to date") || (make  build)

$(LIBFT):
	@make -s -C src/libft 	

$(NAME): $(OBJECTS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJECTS) $(INCFLAGS)
	@echo "\033[33mMaking compile_commands.json...\033[0m"
	@find . -type f -name "compile_commands.json" -delete
	@find ./obj/ -type f -name "*.json" | xargs sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' >> compile_commands.json
	@find ./src/libft/ -type f -name "*.json" | xargs sed -e '1s/^/[\n/' -e '$$s/,$$/\n]/' >> compile_commands.json
	@find ./obj/ -type f -name "*.json" -delete
	@find ./src/libft/ -type f -name "*.json" -delete
	@echo "\033[33mminishell created\033[0m"

obj:
	@mkdir -p obj

obj/%.o: %.c
	$(CC) $(CFLAGS) -MJ $@.json -c -o $@ $< $(INCLUDES)

clean:
	@rm -rf obj
	@if [ -d "obj" ]; then rmdir obj/; \
	fi
	@echo "\033[33mAll object files, dependency files, archive files and test files removed.\033[0m"

fclean: clean
	@rm -rf $(NAME)
	@make -s $@ -C src/libft
	@echo "\033[33mBinary removed.\033[0m"

re: fclean all

.PHONY: all clean fclean re obj 
