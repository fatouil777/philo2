#include "../includes/philo.h"


static t_ms	parse_ms(const char *s)
{
	return ((t_ms)ft_atoi_strict(s));
}

int	validate_args(int ac, char **av, t_prog *prog)
{
	if (ac != 5 && ac != 6)
	{
		write(2, "Error: Wrong number of arguments\n", 33);
		return (1);
	}
	if (!is_positive_number(av[1]) || !is_positive_number(av[2])
		|| !is_positive_number(av[3]) || !is_positive_number(av[4]))
	{
		write(2, "Error: Arguments must be positive integers\n", 42);
		return (1);
	}
	prog->num = ft_atoi_strict(av[1]);
	prog->time_to_die = parse_ms(av[2]);
	prog->time_to_eat = parse_ms(av[3]);
	prog->time_to_sleep = parse_ms(av[4]);
	if (ac == 6)
	{
		if (!is_positive_number(av[5]))
		{
			write(2, "Error: num_times_to_eat must be positive integer\n", 49);
			return (1);
		}
		prog->num_times_must_eat = ft_atoi_strict(av[5]);
	}
	else
		prog->num_times_must_eat = -1;
	return (0);
}

int	init_forks(t_prog *prog)
{
	int	i;

	prog->forks = malloc(sizeof(pthread_mutex_t) * prog->num);
	if (!prog->forks)
		return (1);
	i = 0;
	while (i < prog->num)
	{
		if (pthread_mutex_init(&prog->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	init_program(t_prog *prog)
{
	prog->dead = 0;
	prog->all_ate = 0;
	if (init_forks(prog))
		return (1);
	if (pthread_mutex_init(&prog->write_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&prog->dead_lock, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&prog->meal_lock, NULL) != 0)
		return (1);
	prog->start_time = get_time_ms();
	return (0);
}

int	init_philos(t_philo **philos, t_prog *prog)
{
	int		i;

	*philos = malloc(sizeof(t_philo) * prog->num);
	if (!*philos)
		return (1);
	i = 0;
	while (i < prog->num)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].meals_eaten = 0;
		(*philos)[i].last_meal = prog->start_time;
		(*philos)[i].prog = prog;
		i++;
	}
	/* create philosopher threads */
	i = 0;
	while (i < prog->num)
	{
		if (pthread_create(&(*philos)[i].thread, NULL,
				philo_routine, &(*philos)[i]) != 0)
			return (1);
		if ((i + 1) % 2 == 0)
			usleep(1000);
		i++;
	}
	/* monitor thread (not detached) */
	if (pthread_create(&prog->monitor, NULL, monitor_routine, *philos) != 0)
		return (1);
	return (0);
}
