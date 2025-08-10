#include "../includes/philo.h"


void	destroy_all(t_philo *ph, t_prog *prog)
{
	int	i;

	/* wait for monitor first */
	pthread_join(prog->monitor, NULL);
	/* wait for philosopher threads */
	i = 0;
	while (i < prog->num)
	{
		pthread_join(ph[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < prog->num)
	{
		pthread_mutex_destroy(&prog->forks[i]);
		i++;
	}
	free(prog->forks);
	pthread_mutex_destroy(&prog->write_lock);
	pthread_mutex_destroy(&prog->dead_lock);
	pthread_mutex_destroy(&prog->meal_lock);
	free(ph);
}
