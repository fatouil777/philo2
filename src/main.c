#include "../includes/philo.h"

int	main(int ac, char **av)
{
	t_prog	prog;
	t_philo	*philos;
	int		ret;

	philos = NULL;
	ret = validate_args(ac, av, &prog);
	if (ret)
		return (ret);
	if (init_program(&prog))
		return (1);
	if (init_philos(&philos, &prog))
	{
		destroy_all(philos, &prog);
		return (1);
	}
	/* wait for all threads to finish inside monitor; join done in cleanup */
	destroy_all(philos, &prog);
	return (0);
}
