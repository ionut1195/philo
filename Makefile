NAME = philo
CC = gcc
SRCS = atoi.c philo.c timing.c inits.c input_check.c utils.c
CFLAGS = -Wall -Werror -Wextra -fsanitize=address
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -lpthread

clean:
	rm -f $(OBJS)
fclean:
	rm -f $(NAME)
re: fclean all

.PHONY: all clean fclean re