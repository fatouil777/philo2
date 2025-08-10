#include "../includes/philo.h"


t_ms	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((t_ms)(tv.tv_sec * 1000LL + tv.tv_usec / 1000LL));
}

void	ft_usleep(t_ms ms, t_prog *prog)
{
	t_ms	start;

	start = get_time_ms();
	while ((get_time_ms() - start) < ms)
	{
		usleep(500);
		pthread_mutex_lock(&prog->dead_lock);
		if (prog->dead)
		{
			pthread_mutex_unlock(&prog->dead_lock);
			break ;
		}
		pthread_mutex_unlock(&prog->dead_lock);
	}
}

int	ft_atoi_strict(const char *s)
{
	long long res;
	int			i;

	res = 0;
	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && s[i] <= 13))
		i++;
	while (s[i] >= '0' && s[i] <= '9')
	{
		res = res * 10 + (s[i] - '0');
		i++;
	}
	return ((int)res);
}

int	is_positive_number(const char *s)
{
	int	i;

	if (!s || !s[0])
		return (0);
	i = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	if (ft_atoi_strict(s) <= 0)
		return (0);
	return (1);
}

void print_msg(t_philo *ph, const char *msg)
{
    t_prog *prog = ph->prog;
    
    pthread_mutex_lock(&prog->dead_lock);
    if (prog->dead) {
        pthread_mutex_unlock(&prog->dead_lock);
        return;
    }
    pthread_mutex_unlock(&prog->dead_lock);
    
    pthread_mutex_lock(&prog->write_lock);
    // Check dead again right before printing
    pthread_mutex_lock(&prog->dead_lock);
    if (!prog->dead)
        printf("%lld %d %s\n", get_time_ms() - prog->start_time, ph->id, msg);
    pthread_mutex_unlock(&prog->dead_lock);
    pthread_mutex_unlock(&prog->write_lock);
}
