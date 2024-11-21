# **************************************************************************** #
#                                   VARIABLES                                 #
# **************************************************************************** #

NAME            = minishell

CC              = gcc
CFLAGS          = -Wall -Wextra -Werror -g

# Control verbosity: Set VERBOSE=1 for command output
VERBOSE         = 0
Q               = @
ifeq ($(VERBOSE),1)
Q               =
endif

# Colors for output
RESET_COLOR     = \033[0m
GREEN           = \033[32m
BLUE            = \033[34m
RED             = \033[31m

# Directories
LIBFT_DIR       = ./libft

# Source files
SRCS            = src/main.c\
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

# Object files and dependency files
OBJS            = $(SRCS:.c=.o)
DEPS            = $(SRCS:.c=.d)

# Libraries
LIBFT           = $(LIBFT_DIR)/libft.a
LIBS            = -L$(LIBFT_DIR) -lft -lreadline

# Header files
INCLUDES        = -I. -I$(LIBFT_DIR) -I/usr/include/readline

# **************************************************************************** #
#                                   RULES                                     #
# **************************************************************************** #

# Default target
all: $(NAME)

# Linking the final executable
$(NAME): $(LIBFT) $(OBJS)
	$(Q)echo "Building $(NAME)..."
	$(Q)$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	$(Q)echo "$(GREEN)$(NAME) compiled successfully!$(RESET_COLOR)"

# Building the libft library
$(LIBFT):
	$(Q)echo "Building libft..."
	$(Q)$(MAKE) -C $(LIBFT_DIR) >/dev/null

# Compilation of .c files to .o files with dependency generation
%.o: %.c
	$(Q)$(CC) $(CFLAGS) $(INCLUDES) -MMD -c $< -o $@

# Include dependency files
-include $(DEPS)

# Clean object files
clean:
	$(Q)$(MAKE) -C $(LIBFT_DIR) clean >/dev/null
	$(Q)rm -f $(OBJS) $(DEPS)
	$(Q)echo "$(BLUE)Cleaned object files.$(RESET_COLOR)"

# Clean object files and executable
fclean: clean
	$(Q)$(MAKE) -C $(LIBFT_DIR) fclean >/dev/null
	$(Q)rm -f $(NAME)
	$(Q)echo "$(BLUE)Cleaned all build files.$(RESET_COLOR)"

# Rebuild the project
re: fclean all

# **************************************************************************** #
#                                   PHONY                                     #
# **************************************************************************** #

.PHONY: all clean fclean re
