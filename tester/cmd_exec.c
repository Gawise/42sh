#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

void print_help(char *prog_name)
{
	printf("Usage: %s [-n] DEVNAME COMMAND\n", prog_name);
	printf("Usage: '-n' is an optional argument if you want to push a new line at the end of the text\n");
	printf("Usage: Will require 'sudo' to run if the executable is not setuid root\n");
	exit(1);
}

int main (int argc, char *argv[])
{
	char	*cmd;
	char	*nl;
	int	i;
	int	fd;
	int	devno;
	int	newline;
	int	mem_len;

	devno = 1;
	newline = 0;
	nl = strdup("\n");
	if (argc < 3)
		print_help(argv[0]);
	if (argc > 3 && argv[1][0] == '-' && argv[1][1] == 'n')
	{
		devno = 2;
		newline=1;
	}
	else if (argc > 3 && argv[1][0] == '-' && argv[1][1] != 'n')
	{
		printf("Invalid Option\n");
		print_help(argv[0]);
	}
	fd = open(argv[devno],O_RDWR);
	if (fd == -1)
	{
		perror("open DEVICE");
		exit(1);
	}
	mem_len = strlen(argv[devno + 1]) + 2;
	cmd = (char *)malloc(mem_len);
	strcat(cmd, argv[devno + 1]);
	if (newline == 0)
		usleep(225000);
	i = 0;
	while (cmd[i])
	{
		ioctl (fd, TIOCSTI, cmd + i);
		if (cmd[i++] == '\n')
			usleep(1000000);
	}
	if (newline == 1)
		ioctl (fd, TIOCSTI, nl);
	close(fd);
	free((void *)cmd);
	exit (0);
}
