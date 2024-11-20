# Makefile

# Variables
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Directories
LIBFT_DIR = ./libft

# Source files
SRCS = src/main.c\
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
		src/internal_cmd/internal_commands_utils\
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
		src/parsing/quote.c\
		src/parsing/tokenize/parser.c\
		src/parsing/tokenize/process_word.c\
		src/parsing/tokenize/split_cmd_tokenize.c\
		src/parsing/tokenize/tokenize_cmd.c\
		src/parsing/tokenize/tokenize_input.c\
		src/parsing/tokenize/tokenizer_error.c\
		src/parsing/tokenize/token_utils.c\
		src/processes/process.c\
		src/expand_variable/expand_var_input.c\
		src/expand_variable/expand_var_tokens.c\
		src/expand_variable/variable_expansion.c\
		src/signal_handling/signal_handling.c\
		src/validation/lexer.c

OBJS = $(SRCS:.c=.o)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -L$(LIBFT_DIR) -lft -lreadline

# Header files
INCLUDES = -I. -I$(LIBFT_DIR) -I/usr/include/readline

# Compilation
all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up
clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS)

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

# Phony targets
.PHONY: all clean fclean re
