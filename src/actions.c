#include "../includes/philo.h"


static int	take_forks(t_philo *ph, int left, int right)
{
	t_prog *prog;

	prog = ph->prog;
	if (pthread_mutex_lock(&prog->forks[right]) != 0)
		return (1);
	print_msg(ph, "has taken a fork");
	if (prog->num == 1)
	{
		ft_usleep(prog->time_to_die, prog);
		pthread_mutex_unlock(&prog->forks[right]);
		return (1);
	}
	if (pthread_mutex_lock(&prog->forks[left]) != 0)
	{
		pthread_mutex_unlock(&prog->forks[right]);
		return (1);
	}
	print_msg(ph, "has taken a fork");
	return (0);
}

static void	release_forks(t_philo *ph, int left, int right)
{
	t_prog *prog;

	prog = ph->prog;
	pthread_mutex_unlock(&prog->forks[left]);
	pthread_mutex_unlock(&prog->forks[right]);
}

void	eat_action(t_philo *ph)
{
	t_prog *prog;
	int	left;
	int	right;

	prog = ph->prog;
	left = ph->id - 1;
	right = (ph->id % prog->num);
	if (take_forks(ph, left, right))
		return ;
	pthread_mutex_lock(&prog->meal_lock);
	ph->last_meal = get_time_ms();
	ph->meals_eaten += 1;
	pthread_mutex_unlock(&prog->meal_lock);
	print_msg(ph, "is eating");
	ft_usleep(prog->time_to_eat, prog);
	release_forks(ph, left, right);
}

void	sleep_action(t_philo *ph)
{
	print_msg(ph, "is sleeping");
	ft_usleep(ph->prog->time_to_sleep, ph->prog);
}

void	think_action(t_philo *ph)
{
	print_msg(ph, "is thinking");
}
