/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_actions.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatouil <fatouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:37:21 by fatouil           #+#    #+#             */
/*   Updated: 2025/08/16 22:56:50 by fatouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	perform_thinking_action(t_philosopher *philo)
{
	broadcast_message(THINKING_MSG, philo, philo->philosopher_id);
}

void	perform_sleeping_action(t_philosopher *philo)
{
	broadcast_message(SLEEPING_MSG, philo, philo->philosopher_id);
	precise_sleep(philo->sleeping_duration, philo);
}

void	perform_eating_action(t_philosopher *philo)
{
	pthread_mutex_lock(philo->right_chopstick);
	broadcast_message(FORK_MSG, philo, philo->philosopher_id);
	if (philo->total_philosophers == 1)
	{
		precise_sleep(philo->death_timer, philo);
		pthread_mutex_unlock(philo->right_chopstick);
		return ;
	}
	pthread_mutex_lock(philo->left_chopstick);
	broadcast_message(FORK_MSG, philo, philo->philosopher_id);
	philo->currently_eating = 1;
	broadcast_message(EATING_MSG, philo, philo->philosopher_id);
	pthread_mutex_lock(philo->meal_tracking_mutex);
	philo->timestamp_last_meal = get_timestamp_milliseconds();
	philo->total_meals_consumed++;
	pthread_mutex_unlock(philo->meal_tracking_mutex);
	precise_sleep(philo->eating_duration, philo);
	philo->currently_eating = 0;
	pthread_mutex_unlock(philo->left_chopstick);
	pthread_mutex_unlock(philo->right_chopstick);
	precise_sleep(2, philo);
}