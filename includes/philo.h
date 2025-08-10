#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>

typedef long long t_ms;

typedef struct s_prog
{
	int				num;
	t_ms			time_to_die;
	t_ms			time_to_eat;
	t_ms			time_to_sleep;
	int				num_times_must_eat; /* -1 if not set */
	pthread_mutex_t *forks;
	pthread_mutex_t write_lock;
	pthread_mutex_t dead_lock;
	pthread_mutex_t meal_lock;
	int				dead;
	int				all_ate;
	t_ms			start_time;
	pthread_t		monitor;
}				t_prog;

typedef struct s_philo
{
	int			id;
	int			meals_eaten;
	t_ms		last_meal;
	t_prog		*prog;
	pthread_t	thread;
}				t_philo;

/* main/init */
int		validate_args(int ac, char **av, t_prog *prog);
int		init_program(t_prog *prog);
int		init_forks(t_prog *prog);
int		init_philos(t_philo **philos, t_prog *prog);

/* routine/actions */
void	*philo_routine(void *arg);
void	eat_action(t_philo *ph);
void	sleep_action(t_philo *ph);
void	think_action(t_philo *ph);

/* monitor */
void	*monitor_routine(void *arg);

/* utils */
t_ms	get_time_ms(void);
void	ft_usleep(t_ms ms, t_prog *prog);
int		ft_atoi_strict(const char *s);
int		is_positive_number(const char *s);
void	print_msg(t_philo *ph, const char *msg);

/* cleanup */
void	destroy_all(t_philo *ph, t_prog *prog);

#endif
