NAME = philo
CC = gcc
SRCS = atoi.c philo.c timing.c
CFLAGS = -Wall -Werror -Wextra -g -fsanitize=address
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)
fclean:
	rm -f $(NAME)
re: fclean all

.PHONY: all clean fclean re