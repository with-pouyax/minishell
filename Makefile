# **************************************************************************** #
#                                   VARIABLES                                 #
# **************************************************************************** #

NAME            = minishell

CC              = cc
CFLAGS          = -Wall -Wextra -Werror

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
                  src/execution/init_pipes.c\
                  src/execution/open_redirection_files.c\
                  src/execution/check_redirection.c\
                  src/execution/exec_parent.c\
                  src/execution/handle_heredoc.c\
                  src/helper/helper_1.c\
                  src/helper/helper_2.c\
                  src/heredoc/check_delimiter.c\
                  src/heredoc/expand_heredoc.c\
                  src/heredoc/heredoc.c\
				  src/heredoc/heredoc2.c\
                  src/heredoc/heredoc_error.c\
                  src/heredoc/proccess_heredoc.c\
                  src/heredoc/tmp_file.c\
                  src/internal_cmd/init_internal_cmd.c\
                  src/internal_cmd/internal_commands.c\
				  src/internal_cmd/internal_commands2.c\
                  src/internal_cmd/internal_commands_utils.c\
				  src/internal_cmd/internal_commands_utils2.c\
				  src/internal_cmd/internal_commands_utils3.c\
				  src/internal_cmd/internal_commands_utils4.c\
				  src/internal_cmd/internal_commands_utils5.c\
                  src/internal_cmd/internall_op/ft_cd.c\
				  src/internal_cmd/internall_op/ft_cd2.c\
                  src/internal_cmd/internall_op/handle_cd_errors.c \
                  src/internal_cmd/internall_op/ft_echo.c\
                  src/internal_cmd/internall_op/ft_env.c\
                  src/internal_cmd/internall_op/ft_exit.c\
				  src/internal_cmd/internall_op/ft_exit2.c\
                  src/internal_cmd/internall_op/ft_export.c\
                  src/internal_cmd/internall_op/ft_pwd.c\
                  src/internal_cmd/internall_op/ft_unset.c\
                  src/execution/open_files.c\
                  src/external_commands/external_commands.c\
                  src/external_commands/external_commands2.c\
                  src/external_commands/external_commands3.c\
                  src/external_commands/get_path.c\
                  src/external_commands/get_path2.c\
                  src/external_commands/handel_error_exec.c\
                  src/external_commands/store_pids.c\
                  src/parsing/check_last_pipe.c\
                  src/parsing/handle_input.c\
				  src/parsing/handle_input2.c\
				  src/parsing/handle_input3.c\
                  src/parsing/init_env.c\
                  src/parsing/init_shell.c\
                  src/parsing/quotes.c\
                  src/parsing/check_syntax_error.c\
                  src/parsing/tokenize/parser.c\
                  src/parsing/tokenize/process_word.c\
				  src/parsing/tokenize/process_word2.c\
				  src/parsing/tokenize/process_word3.c\
				  src/parsing/tokenize/process_word4.c\
				  src/parsing/tokenize/process_word5.c\
                  src/parsing/tokenize/split_cmd_tokenize.c\
                  src/parsing/tokenize/tokenize_cmd.c\
                  src/parsing/tokenize/tokenize_input.c\
				  src/parsing/tokenize/tokenize_input2.c\
                  src/parsing/tokenize/tokenizer_error.c\
                  src/parsing/tokenize/token_utils.c\
				  src/parsing/tokenize/token_utils2.c\
				  src/parsing/tokenize/token_utils3.c\
				  src/parsing/tokenize/token_utils4.c\
				  src/parsing/tokenize/token_utils5.c\
				  src/parsing/tokenize/token_utils6.c\
                  src/parsing/tokenize/process_input_segment.c\
                  src/processes/preprocess_input.c\
                  src/processes/expand_variable/expand_var_input.c\
                  src/processes/expand_variable/expand_var_tokens.c\
				  src/processes/expand_variable/expand_var_tokens2.c\
				  src/processes/expand_variable/expand_var_tokens3.c\
				  src/processes/expand_variable/expand_var_tokens4.c\
                  src/processes/expand_variable/variable_expansion.c\
                  src/signal_handling/signal_handling.c\
                  src/validation/lexer.c \
                  src/validation/lexer2.c \
                  src/validation/error.c \
				  src/cleanup/free_redirections.c

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
	$(Q)$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)
	$(Q)echo " "
	$(Q)echo " ▐▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▌"
	$(Q)echo " ▐                                                                                   ▌"
	$(Q)echo " ▐                                                                                   ▌"
	$(Q)echo " ▐   ██████   ██████  ███              ███          █████               ████  ████   ▌"
	$(Q)echo " ▐  ░░██████ ██████  ░░░              ░░░          ░░███               ░░███ ░░███   ▌"
	$(Q)echo " ▐   ░███░█████░███  ████  ████████   ████   █████  ░███████    ██████  ░███  ░███   ▌"
	$(Q)echo " ▐   ░███░░███ ░███ ░░███ ░░███░░███ ░░███  ███░░   ░███░░███  ███░░███ ░███  ░███   ▌"
	$(Q)echo " ▐   ░███ ░░░  ░███  ░███  ░███ ░███  ░███ ░░█████  ░███ ░███ ░███████  ░███  ░███   ▌"
	$(Q)echo " ▐   ░███      ░███  ░███  ░███ ░███  ░███  ░░░░███ ░███ ░███ ░███░░░   ░███  ░███   ▌"
	$(Q)echo " ▐   █████     █████ █████ ████ █████ █████ ██████  ████ █████░░██████  █████ █████  ▌"
	$(Q)echo " ▐  ░░░░░     ░░░░░ ░░░░░ ░░░░ ░░░░░ ░░░░░ ░░░░░░  ░░░░ ░░░░░  ░░░░░░  ░░░░░ ░░░░░   ▌"
	$(Q)echo " ▐                                                                                   ▌"
	$(Q)echo " ▐                                                                                   ▌"
	$(Q)echo " ▐▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▌"
	$(Q)echo " "

# Building the libft library
$(LIBFT):
	$(Q)echo "Building libft..."
	$(Q)$(MAKE) -C $(LIBFT_DIR) >/dev/null

# Compilation of .c files to .o files with dependency generation
%.o: %.c
	$(Q)$(CC) $(CFLAGS) $(INCLUDES)  -c $< -o $@

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