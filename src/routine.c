#include "../includes/philo.h"


void	*philo_routine(void *arg)
{
	t_philo	*ph;
	t_prog	*prog;

	ph = (t_philo *)arg;
	prog = ph->prog;
	while (1)
	{
		pthread_mutex_lock(&prog->dead_lock);
		if (prog->dead)
		{
			pthread_mutex_unlock(&prog->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&prog->dead_lock);
		eat_action(ph);
		pthread_mutex_lock(&prog->meal_lock);
		if (prog->num_times_must_eat > 0
			&& ph->meals_eaten >= prog->num_times_must_eat)
		{
			pthread_mutex_unlock(&prog->meal_lock);
			break ;
		}
		pthread_mutex_unlock(&prog->meal_lock);
		sleep_action(ph);
		pthread_mutex_lock(&prog->dead_lock);
		if (prog->dead)
		{
			pthread_mutex_unlock(&prog->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&prog->dead_lock);
		think_action(ph);
	}
	return (NULL);
}
