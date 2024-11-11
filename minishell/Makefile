# Variables
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

# Directories
LIBFT_DIR = ./libft

# Source files
SRCS = main.c heredoc.c preprocess_input.c handle_input.c init_shell.c helpers.c lexer.c \
       parser.c token_utils.c variable_expansion.c print_tokens.c \
       free_commands.c tokenize_input.c tokenize_input_utils.c execute_commands.c internal_commands.c redirection.c

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
