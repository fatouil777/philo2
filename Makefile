NAME = philo
SRC = main.c utils.c init.c threads.c monitor.c routine_actions.c

FLAGS = -Wall -Wextra -Werror -O3 -pthread


all: $(NAME)

$(NAME): $(SRC) 
	@cc $(FLAGS) -o $(NAME) $(SRC) $(SANITIZER)

clean:
	@rm -f $(NAME)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug
