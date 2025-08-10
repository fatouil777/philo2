#include "../includes/philo.h"

void	*monitor_routine(void *arg)
{
	t_philo	*ph;
	t_prog	*prog;
	int		i;
	int		count;

	ph = (t_philo *)arg;
	prog = ph->prog;
	while (1)
	{
		i = 0;
		count = 0;
		while (i < prog->num)
		{
			pthread_mutex_lock(&prog->meal_lock);
			/* check death */
        if ((get_time_ms() - ph[i].last_meal) >= prog->time_to_die)
        {
            pthread_mutex_lock(&prog->dead_lock);
            if (!prog->dead)
            {
                prog->dead = 1;
                pthread_mutex_lock(&prog->write_lock); // ADD THIS
                printf("%lld %d died\n",
                    (get_time_ms() - prog->start_time), ph[i].id);
                pthread_mutex_unlock(&prog->write_lock); // ADD THIS
            }
            pthread_mutex_unlock(&prog->dead_lock);
            pthread_mutex_unlock(&prog->meal_lock);
            return (NULL);
}
			/* count full philosophers when num_times_must_eat set */
			if (prog->num_times_must_eat > 0
				&& ph[i].meals_eaten >= prog->num_times_must_eat)
				count++;
			pthread_mutex_unlock(&prog->meal_lock);
			i++;
		}
		if (prog->num_times_must_eat > 0 && count == prog->num)
		{
			pthread_mutex_lock(&prog->dead_lock);
			prog->all_ate = 1;
			pthread_mutex_unlock(&prog->dead_lock);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}
