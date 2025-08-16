/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatouil <fatouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:36:57 by fatouil           #+#    #+#             */
/*   Updated: 2025/08/16 23:00:09 by fatouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	parse_input_parameters(t_philosopher *philo, char **arguments)
{
	philo->death_timer = string_to_integer(arguments[2]);
	philo->eating_duration = string_to_integer(arguments[3]);
	philo->sleeping_duration = string_to_integer(arguments[4]);
	philo->total_philosophers = string_to_integer(arguments[1]);
	if (arguments[5])
		philo->required_meal_count = string_to_integer(arguments[5]);
	else
		philo->required_meal_count = -1;
}

void	configure_philosophers(t_philosopher *philosophers, t_simulation *sim,
		pthread_mutex_t *chopsticks, char **arguments)
{
	int	current_philosopher;

	current_philosopher = 0;
	while (current_philosopher < string_to_integer(arguments[1]))
	{
		philosophers[current_philosopher].philosopher_id = current_philosopher + 1;
		philosophers[current_philosopher].currently_eating = 0;
		philosophers[current_philosopher].total_meals_consumed = 0;
		parse_input_parameters(&philosophers[current_philosopher], arguments);
		philosophers[current_philosopher].simulation_start_time = get_timestamp_milliseconds();
		philosophers[current_philosopher].timestamp_last_meal = get_timestamp_milliseconds();
		philosophers[current_philosopher].output_mutex = &sim->output_mutex;
		philosophers[current_philosopher].death_check_mutex = &sim->death_check_mutex;
		philosophers[current_philosopher].meal_tracking_mutex = &sim->meal_tracking_mutex;
		philosophers[current_philosopher].simulation_ended = &sim->end_simulation;
		philosophers[current_philosopher].left_chopstick = &chopsticks[current_philosopher];
		if (current_philosopher == 0)
			philosophers[current_philosopher].right_chopstick = 
				&chopsticks[philosophers[current_philosopher].total_philosophers - 1];
		else
			philosophers[current_philosopher].right_chopstick = 
				&chopsticks[current_philosopher - 1];
		current_philosopher++;
	}
}

void	initialize_chopsticks(pthread_mutex_t *chopsticks, int philosopher_count)
{
	int	chopstick_index;

	chopstick_index = 0;
	while (chopstick_index < philosopher_count)
	{
		pthread_mutex_init(&chopsticks[chopstick_index], NULL);
		chopstick_index++;
	}
}

void	setup_simulation(t_simulation *sim, t_philosopher *philosophers)
{
	sim->end_simulation = 0;
	sim->philosophers_array = philosophers;
	pthread_mutex_init(&sim->output_mutex, NULL);
	pthread_mutex_init(&sim->death_check_mutex, NULL);
	pthread_mutex_init(&sim->meal_tracking_mutex, NULL);
}
