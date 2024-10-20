# Variables
NAME = minishell
CC = gcc
CFLAGS = -Wall -Wextra -Werror

# Directories
LIBFT_DIR = ./libft
GNL_DIR = ./get_next_line

# Source files
SRCS = main.c lexer.c parser.c print_tokens.c token_utils.c $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c
OBJS = $(SRCS:.c=.o)

# Libraries
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -L$(LIBFT_DIR) -lft -lreadline

# Header files
INCLUDES = -I. -I$(LIBFT_DIR) -I$(GNL_DIR)

# Compilation
all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

%.o: %.c
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
