

#include "philosophers.h"

int	check_simulation_status(t_philosopher *philo)
{
	pthread_mutex_lock(philo->death_check_mutex);
	if (*philo->simulation_ended == 1)
		return (pthread_mutex_unlock(philo->death_check_mutex), 1);
	pthread_mutex_unlock(philo->death_check_mutex);
	return (0);
}

void	*philosopher_lifecycle(void *philosopher_data)
{
	t_philosopher	*current_philosopher;

	current_philosopher = (t_philosopher *)philosopher_data;
	if (current_philosopher->philosopher_id % 2 == 0)
		precise_sleep(1);
	while (!check_simulation_status(current_philosopher))
	{
		perform_eating_action(current_philosopher);
		perform_sleeping_action(current_philosopher);
		perform_thinking_action(current_philosopher);
	}
	return (philosopher_data);
}

int	launch_simulation(t_simulation *sim, pthread_mutex_t *chopsticks)
{
	pthread_t	monitoring_thread;
	int			philosopher_index;

	if (pthread_create(&monitoring_thread, NULL, &monitor_philosophers, 
		sim->philosophers_array) != 0)
		cleanup_resources("Error: Failed to create monitoring thread", 
			sim, chopsticks);
	philosopher_index = 0;
	while (philosopher_index < sim->philosophers_array[0].total_philosophers)
	{
		if (pthread_create(&sim->philosophers_array[philosopher_index].life_thread, 
			NULL, &philosopher_lifecycle, 
			&sim->philosophers_array[philosopher_index]) != 0)
			cleanup_resources("Error: Failed to create philosopher thread", 
				sim, chopsticks);
		philosopher_index++;
	}
	philosopher_index = 0;
	if (pthread_join(monitoring_thread, NULL) != 0)
		cleanup_resources("Error: Failed to join monitoring thread", 
			sim, chopsticks);
	while (philosopher_index < sim->philosophers_array[0].total_philosophers)
	{
		if (pthread_join(sim->philosophers_array[philosopher_index].life_thread, 
			NULL) != 0)
			cleanup_resources("Error: Failed to join philosopher thread", 
				sim, chopsticks);
		philosopher_index++;
	}
	return (0);
}
