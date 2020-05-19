#include <unistd.h>
#include <stdlib.h>

int		main(int ac, char **av)
{
	if (ac == 1)
	{
		sleep(60);
		return (0);
	}
	int s = atoi(av[1]);
	if (s)
	{
		sleep(s);
		return (0);
	}
	return (1);
}
