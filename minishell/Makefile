# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/27 12:00:00 by pghajard          #+#    #+#              #
#    Updated: 2024/11/20 19:57:20 by pghajard         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
#                                   VARIABLES                                 #
# **************************************************************************** #

NAME			= minishell

CC				= gcc
CFLAGS			= -Wall -Wextra -Werror -g

# Directories
LIBFT_DIR		= ./libft

# Source files
SRCS			= src/main.c\
					src/cleanup/cleanup.c\
					src/cleanup/free_commands.c\
					src/execution/execute_commands.c\
					src/execution/external_commands.c\
					src/execution/init_pipes.c\
					src/helper/helper_1.c\
					src/helper/helper_2.c\
					src/heredoc/check_delimiter.c\
					src/heredoc/expand_heredoc.c\
					src/heredoc/heredoc.c\
					src/heredoc/heredoc_error.c\
					src/heredoc/proccess_heredoc.c\
					src/heredoc/tmp_file.c\
					src/internal_cmd/init_internal_cmd.c\
					src/internal_cmd/internal_commands.c\
					src/internal_cmd/internal_commands_utils.c\
					src/internal_cmd/internall_op/ft_cd.c\
					src/internal_cmd/internall_op/ft_echo.c\
					src/internal_cmd/internall_op/ft_env.c\
					src/internal_cmd/internall_op/ft_exit.c\
					src/internal_cmd/internall_op/ft_export.c\
					src/internal_cmd/internall_op/ft_pwd.c\
					src/internal_cmd/internall_op/ft_unset.c\
					src/parsing/check_last_pipe.c\
					src/parsing/handle_input.c\
					src/parsing/init_env.c\
					src/parsing/init_shell.c\
					src/parsing/quotes.c\
					src/parsing/tokenize/parser.c\
					src/parsing/tokenize/process_word.c\
					src/parsing/tokenize/split_cmd_tokenize.c\
					src/parsing/tokenize/tokenize_cmd.c\
					src/parsing/tokenize/tokenize_input.c\
					src/parsing/tokenize/tokenizer_error.c\
					src/parsing/tokenize/token_utils.c\
					src/processes/process.c\
					src/processes/preprocess_input.c\
					src/processes/expand_variable/expand_var_input.c\
					src/processes/expand_variable/expand_var_tokens.c\
					src/processes/expand_variable/variable_expansion.c\
					src/signal_handling/signal_handling.c\
					src/validation/lexer.c \
					print_tokens.c
# Object files
OBJS			= $(SRCS:.c=.o)

# Libraries
LIBFT			= $(LIBFT_DIR)/libft.a
LIBS			= -L$(LIBFT_DIR) -lft -lreadline

# Header files
INCLUDES		= -I. -I$(LIBFT_DIR) -I/usr/include/readline

# **************************************************************************** #
#                                   RULES                                     #
# **************************************************************************** #

# Default target
all: $(NAME)

# Linking the final executable
$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

# Building the libft library
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Compilation of .c files to .o files
%.o: %.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean object files
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

# Clean object files and executable
fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

# Rebuild the project
re: fclean all

# **************************************************************************** #
#                                   PHONY                                     #
# **************************************************************************** #

.PHONY: all clean fclean re
