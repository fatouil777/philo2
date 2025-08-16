/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatouil <fatouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:37:04 by fatouil           #+#    #+#             */
/*   Updated: 2025/08/16 22:51:25 by fatouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	validate_argument_content(char *argument)
{
	int	index;

	index = 0;
	while (argument[index] != '\0')
	{
		if (argument[index] < '0' || argument[index] > '9')
			return (1);
		index++;
	}
	return (0);
}

int	validate_all_arguments(char **arguments)
{
	if (string_to_integer(arguments[1]) > MAX_PHILOSOPHERS 
		|| string_to_integer(arguments[1]) <= 0
		|| validate_argument_content(arguments[1]) == 1)
	{
		write_to_file_descriptor("Error: Invalid number of philosophers\n", 2);
		return (1);
	}
	if (string_to_integer(arguments[2]) <= 0 
		|| validate_argument_content(arguments[2]) == 1)
	{
		write_to_file_descriptor("Error: Invalid time to die\n", 2);
		return (1);
	}
	if (string_to_integer(arguments[3]) <= 0 
		|| validate_argument_content(arguments[3]) == 1)
	{
		write_to_file_descriptor("Error: Invalid time to eat\n", 2);
		return (1);
	}
	if (string_to_integer(arguments[4]) <= 0 
		|| validate_argument_content(arguments[4]) == 1)
	{
		write_to_file_descriptor("Error: Invalid time to sleep\n", 2);
		return (1);
	}
	if (arguments[5] && (string_to_integer(arguments[5]) < 0 
		|| validate_argument_content(arguments[5]) == 1))
	{
		write_to_file_descriptor("Error: Invalid meal count requirement\n", 2);
		return (1);
	}
	return (0);
}

int	main(int argument_count, char **arguments)
{
	t_simulation		dining_simulation;
	t_philosopher		philosophers[MAX_PHILOSOPHERS];
	pthread_mutex_t		chopsticks[MAX_PHILOSOPHERS];

	if (argument_count != 5 && argument_count != 6)
	{
		write_to_file_descriptor("Usage: ./philo [num_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [optional: meals_required]\n", 2);
		return (1);
	}
	if (validate_all_arguments(arguments) == 1)
		return (1);
	setup_simulation(&dining_simulation, philosophers);
	initialize_chopsticks(chopsticks, string_to_integer(arguments[1]));
	configure_philosophers(philosophers, &dining_simulation, chopsticks, arguments);
	launch_simulation(&dining_simulation, chopsticks);
	cleanup_resources(NULL, &dining_simulation, chopsticks);
	return (0);
}
