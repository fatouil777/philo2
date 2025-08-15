

#include "philosophers.h"

void	broadcast_message(char *message, t_philosopher *philo, int id)
{
	size_t	elapsed_time;

	pthread_mutex_lock(philo->output_mutex);
	elapsed_time = get_timestamp_milliseconds() - philo->simulation_start_time;
	if (!check_simulation_status(philo))
		printf("%zu %d %s\n", elapsed_time, id, message);
	pthread_mutex_unlock(philo->output_mutex);
}

int	is_philosopher_starved(t_philosopher *philo, size_t death_limit)
{
	pthread_mutex_lock(philo->meal_tracking_mutex);
	if (get_timestamp_milliseconds() - philo->timestamp_last_meal >= death_limit + 1
		&& philo->currently_eating == 0)
		return (pthread_mutex_unlock(philo->meal_tracking_mutex), 1);
	pthread_mutex_unlock(philo->meal_tracking_mutex);
	return (0);
}

int	check_for_philosopher_death(t_philosopher *philosophers)
{
	int	philosopher_index;

	philosopher_index = 0;
	while (philosopher_index < philosophers[0].total_philosophers)
	{
		if (is_philosopher_starved(&philosophers[philosopher_index], 
			philosophers[philosopher_index].death_timer))
		{
			broadcast_message(DEATH_MSG, &philosophers[philosopher_index], 
				philosophers[philosopher_index].philosopher_id);
			pthread_mutex_lock(philosophers[0].death_check_mutex);
			*philosophers->simulation_ended = 1;
			pthread_mutex_unlock(philosophers[0].death_check_mutex);
			return (1);
		}
		philosopher_index++;
	}
	return (0);
}

int	verify_all_philosophers_fed(t_philosopher *philosophers)
{
	int	philosopher_index;
	int	philosophers_who_finished_eating;

	philosopher_index = 0;
	philosophers_who_finished_eating = 0;
	if (philosophers[0].required_meal_count == -1)
		return (0);
	while (philosopher_index < philosophers[0].total_philosophers)
	{
		pthread_mutex_lock(philosophers[philosopher_index].meal_tracking_mutex);
		if (philosophers[philosopher_index].total_meals_consumed >= 
			philosophers[philosopher_index].required_meal_count)
			philosophers_who_finished_eating++;
		pthread_mutex_unlock(philosophers[philosopher_index].meal_tracking_mutex);
		philosopher_index++;
	}
	if (philosophers_who_finished_eating == philosophers[0].total_philosophers)
	{
		pthread_mutex_lock(philosophers[0].death_check_mutex);
		*philosophers->simulation_ended = 1;
		pthread_mutex_unlock(philosophers[0].death_check_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_philosophers(void *simulation_data)
{
	t_philosopher	*philosophers_array;

	philosophers_array = (t_philosopher *)simulation_data;
	while (1)
	{
		if (check_for_philosopher_death(philosophers_array) == 1 
			|| verify_all_philosophers_fed(philosophers_array) == 1)
			break ;
	}
	return (simulation_data);
}
