/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatouil <fatouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:37:32 by fatouil           #+#    #+#             */
/*   Updated: 2025/08/16 22:55:24 by fatouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	write_to_file_descriptor(char *str, int fd)
{
	if (write(fd, str, calculate_string_length(str)) == -1)
	{
		/* Error handling could be implemented here if needed */
	}
}

int	calculate_string_length(char *str)
{
	int	length;

	if (str == NULL)
		return (0);
	length = 0;
	while (str[length] != '\0')
		length++;
	return (length);
}

int	string_to_integer(char *str)
{
	unsigned long long	number;
	int					sign_multiplier;
	int					char_index;

	number = 0;
	sign_multiplier = 1;
	char_index = 0;
	while (str[char_index] == ' ' || str[char_index] == '\t' 
		|| str[char_index] == '\n' || str[char_index] == '\v'
		|| str[char_index] == '\f' || str[char_index] == '\r')
		char_index++;
	if (str[char_index] == '-')
		sign_multiplier = -1;
	if (str[char_index] == '-' || str[char_index] == '+')
		char_index++;
	while (str[char_index] >= '0' && str[char_index] <= '9')
	{
		number = number * 10 + (str[char_index] - '0');
		char_index++;
	}
	return (sign_multiplier * number);
}

void	cleanup_resources(char *error_msg, t_simulation *sim, 
		pthread_mutex_t *chopsticks)
{
	int	resource_index;

	resource_index = 0;
	if (error_msg)
	{
		write_to_file_descriptor(error_msg, 2);
		write_to_file_descriptor("\n", 2);
	}
	pthread_mutex_destroy(&sim->output_mutex);
	pthread_mutex_destroy(&sim->meal_tracking_mutex);
	pthread_mutex_destroy(&sim->death_check_mutex);
	while (resource_index < sim->philosophers_array[0].total_philosophers)
	{
		pthread_mutex_destroy(&chopsticks[resource_index]);
		resource_index++;
	}
}

int	precise_sleep(size_t milliseconds, t_philosopher *philo)
{
	size_t	sleep_start_time;

	sleep_start_time = get_timestamp_milliseconds();
	while (!check_simulation_status(philo) && (get_timestamp_milliseconds() - sleep_start_time) < milliseconds)
		usleep(500);
	return (0);
}

size_t	get_timestamp_milliseconds(void)
{
	struct timeval	current_time;

	if (gettimeofday(&current_time, NULL) == -1)
		write_to_file_descriptor("Error: gettimeofday() failed\n", 2);
	return (current_time.tv_sec * 1000 + current_time.tv_usec / 1000);
}