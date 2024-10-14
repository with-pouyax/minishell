# Name of the final executable
NAME = minishell

# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(READLINE_DIR)/include

# Readline library paths
READLINE_DIR = /usr/local/opt/readline
LIBS = -L$(READLINE_DIR)/lib -lreadline

# List of source files
SRC_FILES = minishell.c \
            init.c \
            builtins.c \
            builtins_extra.c \
            env_utils.c \
            env_vars.c \
            expand_vars.c \
            executor.c \
            heredoc.c \
            history.c \
            lexer.c \
            lexer_extra.c \
            parser.c \
            parser_extra.c \
            signal_handling.c \
            utils.c \
            utils_extra.c \
            utils_more.c \
            utils_array.c

# List of object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Compile the final executable
$(NAME): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(LIBS) -o $(NAME)

# Compile source files into object files
%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Default target
all: $(NAME)

# Clean object files
clean:
	rm -f $(OBJ_FILES)

# Clean object files and executable
fclean: clean
	rm -f $(NAME)

# Rebuild the project
re: fclean all

.PHONY: all clean fclean re
