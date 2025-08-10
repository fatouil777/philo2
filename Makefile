NAME = philo
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pthread -I include
SRCS = src/main.c src/init.c src/utils.c src/routine.c src/actions.c \
	   src/monitor.c src/cleanup.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
