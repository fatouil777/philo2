/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fatouil <fatouil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 21:37:16 by fatouil           #+#    #+#             */
/*   Updated: 2025/08/16 22:56:18 by fatouil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

# define MAX_PHILOSOPHERS 300
# define THINKING_MSG "is thinking"
# define SLEEPING_MSG "is sleeping"
# define EATING_MSG "is eating"
# define FORK_MSG "has taken a fork"
# define DEATH_MSG "died"

typedef struct s_philosopher
{
	pthread_t		life_thread;
	int				philosopher_id;
	int				currently_eating;
	int				total_meals_consumed;
	size_t			timestamp_last_meal;
	size_t			death_timer;
	size_t			eating_duration;
	size_t			sleeping_duration;
	size_t			simulation_start_time;
	int				total_philosophers;
	int				required_meal_count;
	int				*simulation_ended;
	pthread_mutex_t	*right_chopstick;
	pthread_mutex_t	*left_chopstick;
	pthread_mutex_t	*output_mutex;
	pthread_mutex_t	*death_check_mutex;
	pthread_mutex_t	*meal_tracking_mutex;
}					t_philosopher;

typedef struct s_simulation
{
	int				end_simulation;
	pthread_mutex_t	death_check_mutex;
	pthread_mutex_t	meal_tracking_mutex;
	pthread_mutex_t	output_mutex;
	t_philosopher	*philosophers_array;
}					t_simulation;

// === CORE FUNCTIONS ===
int					validate_argument_content(char *argument);
int					validate_all_arguments(char **arguments);
void				cleanup_resources(char *error_msg, t_simulation *sim,
						pthread_mutex_t *chopsticks);

// === INITIALIZATION FUNCTIONS ===
void				setup_simulation(t_simulation *sim, t_philosopher *philos);
void				initialize_chopsticks(pthread_mutex_t *chopsticks, 
						int philosopher_count);
void				configure_philosophers(t_philosopher *philos, 
						t_simulation *sim, pthread_mutex_t *chopsticks, 
						char **arguments);
void				parse_input_parameters(t_philosopher *philo, char **arguments);

// === THREAD MANAGEMENT ===
int					launch_simulation(t_simulation *sim, 
						pthread_mutex_t *chopsticks);
void				*monitor_philosophers(void *simulation_data);
void				*philosopher_lifecycle(void *philosopher_data);

// === PHILOSOPHER ACTIONS ===
void				perform_eating_action(t_philosopher *philo);
void				perform_sleeping_action(t_philosopher *philo);
void				perform_thinking_action(t_philosopher *philo);

// === MONITORING UTILITIES ===
int					check_simulation_status(t_philosopher *philo);
int					verify_all_philosophers_fed(t_philosopher *philos);
int					check_for_philosopher_death(t_philosopher *philos);
int					is_philosopher_starved(t_philosopher *philo, 
						size_t death_limit);

// === UTILITY FUNCTIONS ===
int					string_to_integer(char *str);
int					precise_sleep(size_t milliseconds, t_philosopher *philo);
int					calculate_string_length(char *str);
void				write_to_file_descriptor(char *str, int fd);
void				broadcast_message(char *message, t_philosopher *philo, 
						int id);
size_t				get_timestamp_milliseconds(void);

#endif
