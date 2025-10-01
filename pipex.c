/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:37:45 by djareno           #+#    #+#             */
/*   Updated: 2025/10/01 15:13:01 by djareno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *find_cmd(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;
	int		i;
	char	*path_env;

	path_env = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_env = envp[i] + 5;
			break ;
		}
		i++;
	}
	paths = ft_split(envp[i], ':');
	if (!paths)
		return (NULL);
	i = 0;
	tmp = ft_strjoin("/", cmd);
	if (!tmp)
		return (ft_free_matrix(paths), NULL);
	while (paths[i])
	{
		path = ft_strjoin(paths[i], tmp);
		if (!path)
			return (ft_free_matrix(paths), free(tmp), NULL);
		if (access(path, F_OK) == 0)
		{
			ft_free_matrix(paths);
			free(tmp);
			return (path);
		}
		free (path);
		i++;
	}
	free (tmp);
	ft_free_matrix(paths);
	return (NULL);
}

t_arglist	*parse_args(int argc, char **argv, char **envp)
{
	t_arglist	*arglist;

	arglist = (t_arglist *) ft_calloc(1, sizeof(t_arglist));
	arglist->arg1 = NULL;
	arglist->arg2 = NULL;
	if (argc != 5)
	{
		ft_putstr_fd("Error, the arguments should be:\n", 2);
		return (ft_putstr_fd("infile command1 command2 outfile", 2), NULL);
	}
	if (access(argv[1], F_OK) != 0)
		return (ft_putstr_fd("Error, invalid infile", 2), NULL);
	if (access(argv[1], R_OK) != 0)
		return (ft_putstr_fd("Not enugh rights to access infile\n", 2), NULL);
	if (access(argv[4], F_OK) == 0 && access(argv[1], R_OK) != 0)
		return (ft_putstr_fd("Not enugh rights to access outfile\n", 2), NULL);
	arglist->arg1 = ft_split(argv[2], ' ');
	arglist->arg2 = ft_split(argv[3], ' ');
	if (access(find_cmd(arglist->arg1[0], envp), F_OK) != 0)
		return (ft_putstr_fd("Could not find command\n", 2), NULL);
	if (access(find_cmd(arglist->arg2[0], envp), F_OK) != 0)
		return (ft_putstr_fd("Could not find command\n", 2), NULL);
	return (arglist);
}

int	main(int argc, char **argv, char **envp)
{
	int			fd[2];
	pid_t		pid1;
	pid_t		pid2;
	t_arglist	*arglist;

	arglist = parse_args(argc, argv, envp);
	if (!arglist)
		return (0);
	arglist->infile = open(argv[1], O_RDONLY);
	arglist->outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	pipe(fd);
	pid1 = fork();
	if (pid1 == 0)
	{
		dup2(arglist->infile, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		execve(find_cmd(arglist->arg1[0], envp), arglist->arg1, envp);
		exit(1);
	}
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(arglist->outfile, STDOUT_FILENO);
		close(fd[1]);
		close(fd[0]);
		execve(find_cmd(arglist->arg2[0], envp), arglist->arg2, envp);
		exit(1);
	}
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
