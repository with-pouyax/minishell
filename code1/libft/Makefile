# Compiler
CC = cc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Name of the library 
NAME = libft.a

# Source files (.c)
SRCS = ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c \
       ft_memmove.c ft_strlcpy.c ft_strlcat.c ft_toupper.c \
       ft_tolower.c ft_strchr.c ft_strrchr.c ft_strncmp.c \
       ft_memchr.c ft_memcmp.c ft_strnstr.c ft_atoi.c \
       ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c \
       ft_isprint.c ft_calloc.c ft_strdup.c ft_substr.c ft_putnbr_fd.c\
       ft_putendl_fd.c ft_putstr_fd.c ft_putchar_fd.c ft_strtrim.c ft_split.c\
	   ft_itoa.c ft_strmapi.c ft_striteri.c ft_strjoin.c\
	   
# Object files (.o)
OBJS = $(SRCS:.c=.o)

# Rule to make the library
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

# Rule to remove object files
clean:
	rm -f $(OBJS)

# Rule to remove the library and object files
fclean: clean
	rm -f $(NAME)

# Rule to re-make everything
re: fclean all

# Phony targets
.PHONY: all clean fclean re bonus
