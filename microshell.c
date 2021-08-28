#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>

char	**g_envp;

void	ft_put_str_err(char *str)
{
	int		counter;

	counter = 0;
	while (str[counter] != '\0')
		++counter;
	write (2, str, counter);
}

int		ft_cd(char **comm, int comm_amnt)
{
	if (comm_amnt != 2)
	{
		ft_put_str_err("error: cd: bad arguments\n");
		return (1);
	}
	if (chdir(*++comm) != 0)
	{
		ft_put_str_err("error: cd: cannot change directory to ");
		ft_put_str_err(*comm);
		ft_put_str_err("\n");
		return (1);
	}
	return (0);
}

char	**find_next_pipe(char **comm, int comm_amnt)
{
	while (comm_amnt-- > 0)
	{
		if (strcmp(*comm, "|") == 0)
			return (comm);
		++comm;
	}
	return (NULL);
}

void	cpy_array(char **dst, char **src, int elem_amnt)
{
	while (elem_amnt-- > 0)
	{
		*dst = *src;
		++dst;
		++src;
	}
	*dst = NULL;
}

int		call_comm(char **comm, int comm_amnt)
{
	char	**param;
	pid_t pid;
	int		status;

	if (strcmp(*comm, "cd") == 0)
		return (ft_cd(comm, comm_amnt));
	pid = fork();
	if (pid < 0)
	{
		ft_put_str_err("error: fatal\n");
		exit(1);
	}
	if (pid == 0)
	{
		param = (char **)malloc(sizeof (char*) * comm_amnt + 1);
		if (param == NULL)
		{
			ft_put_str_err("error: fatal\n");
			exit (-1);
		}
		cpy_array(param, comm, comm_amnt);
		status = execve(*comm, param, g_envp);
		ft_put_str_err("error: cannot execute ");
		ft_put_str_err(*comm);
		ft_put_str_err("\n");
		exit (status);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status) != 0)
			return (WEXITSTATUS(status));
		return (-1);
	}
}

int		exec_all(char **comm, int comm_amnt)
{
	char	**next_pipe;
	pid_t	pid;
	int		pipe_fds[2];

	next_pipe = find_next_pipe(comm, comm_amnt);
	if (next_pipe != NULL)
	{
		if (pipe(pipe_fds) != 0)
		{
			ft_put_str_err("error: fatal\n");
			exit (-1);
		}
		pid = fork();
		if (pid < 0)
		{
			ft_put_str_err("error: fatal\n");
			exit (-1);
		}
		if (pid == 0)
		{
			dup2(pipe_fds[0], 0);
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			exec_all(next_pipe + 1, comm_amnt - (next_pipe - comm));
			exit(0);
		}
		dup2(pipe_fds[1], 1);
		close(pipe_fds[1]);
		close(pipe_fds[0]);
		exec_all(comm, next_pipe - comm);
		close(1);
		waitpid(pid, NULL, 0);
		return (0);
	}
	return (call_comm(comm, comm_amnt));
}

int		main(int argc, char **argv, char **envp)
{
	char	**first_comm;

	if (argc-- < 1)
		return (0);
	g_envp = envp;
	first_comm = ++argv;
	while (argc-- > 0)
	{
		if (strcmp(*argv, ";") == 0)
		{
			exec_all(first_comm, argv - first_comm);
			first_comm = argv + 1;
		}
		++argv;
	}
	return (exec_all(first_comm, argv - first_comm));
}