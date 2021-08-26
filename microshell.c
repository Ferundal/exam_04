#include <unistd.h>
#include <string.h>
#include <stdlib.h>

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

char	**find_last_pipe(char **comm, int comm_amnt)
{
	char	**last_pipe;

	last_pipe = NULL;
	while (comm_amnt-- > 0)
	{
		if (strcmp(*comm, "|") == 0)
			last_pipe = comm;
		++comm;
	}
	return (last_pipe);
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
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		status = execve(*comm, param, g_envp);
		ft_put_str_err("error: cannot execute ");
		ft_put_str_err(*comm);
		ft_put_str_err("\n");
		exit (status);
	}
	else
	{
		waitpid(0, &status, 0);
		if (WIFEXITED(status) != 0)
			return (WEXITSTATUS(status));
		return (-1);
	}
}

int		exec_all(char **comm, int comm_amnt)
{
	char	**last_pipe;
	int		pipe_fd_arr[2];
	pid_t	pid;
	int		status;
	int	temp_fd;

	last_pipe = find_last_pipe(comm, comm_amnt);
	if (last_pipe != NULL)
	{
		pipe (pipe_fd_arr);
		pid = fork();
		if (pid < 0)
		{
			ft_put_str_err("error: fatal\n");
			exit(1);
		}
		if (pid == 0)
		{
			dup2(pipe_fd_arr[1], 1);
			close(pipe_fd_arr[0]);
			close(pipe_fd_arr[1]);
			status = exec_all(comm, last_pipe - comm);
			exit (status);
		}
		else
		{
			temp_fd = dup(0);
			dup2(pipe_fd_arr[0], 0);
			close(pipe_fd_arr[0]);
			close(pipe_fd_arr[1]);
			status = call_comm(last_pipe + 1, comm_amnt - (last_pipe -1 - comm));
			dup2(temp_fd, 0);
			close(temp_fd);
			return (status);
		}
	}
	else
		return (call_comm(comm, comm_amnt));
}

int		main(int argc, char **argv, char **envp)
{
	char	**first_comm;

	if (argc-- < 1)
		return (0);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
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