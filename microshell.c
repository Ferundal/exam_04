#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int		ft_cd

char	**find_last_pipe(char **comm, int comm_amnt)
{
	char	**last_pipe;

	last_pipe = NULL;
	while (comm_amnt-- > 0)
	{
		if (strcmp(*comm, "|") == 0)
			last_pipe = comm;
	}
	return (last_pipe);
}

int		call_comm(char **comm, int comm_amnt)
{
	pid_t pid;


	if (strcmp(*comm, "cd") == 0)
		return (ft_cd);
	pid = fork();
	if (pid < 0)
	{
		write(2, "error: fatal\n", 12);
		exit(1);
	}
	if (pid == 0)
	{
		return (st)
	}
	else
	{

	}
}

int		exec_all(char **comm, int comm_amnt)
{
	char	**last_pipe;

	last_pipe = find_last_pipe(comm, comm_amnt);
	while (last_pipe != NULL)
	{

		last_pipe = find_last_pipe(comm, comm_amnt);
	}
	return(ex)
}

int		main(int argc, char **argv, char **envp)
{
	char	**first_comm;

	if (argc-- < 1)
		return (0);
	first_comm = ++argv;
	while (argc-- > 0)
	{
		if (strcmp(*argv, ";") == 0)
		{
			exec_all(first_comm, argv - first_comm);
			first_comm = argv + 1;
		}
	}
	return (exec_all(first_comm, argv - first_comm);
}