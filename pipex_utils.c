/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:09:18 by djareno           #+#    #+#             */
/*   Updated: 2025/10/17 10:32:54 by djareno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = envp[i] + 5;
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	*find_cmd(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;
	int		i;

	if (!cmd || *cmd == '\0')
		return (NULL);
	paths = ft_split(get_path(envp), ':');
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
			return (free(tmp), ft_free_matrix(paths), ft_strdup(""));
		if (access(path, F_OK) == 0)
			return (ft_free_matrix(paths), free(tmp), path);
		free(path);
		i++;
	}
	return (free (tmp), ft_free_matrix(paths), ft_strdup(""));
}

t_arglist	*parse_args(int argc, char **argv)
{
	t_arglist	*arglist;

	arglist = (t_arglist *) ft_calloc(1, sizeof(t_arglist));
	arglist->error = 1;
	if (argc != 5)
	{
		free(arglist);
		ft_putstr_fd("Error, the arguments should be:\n", 2);
		return (ft_putstr_fd("infile command1 command2 outfile\n", 2), NULL);
	}
	if (access(argv[1], F_OK) != 0)
		ft_putstr_fd("Error, invalid infile\n", 2);
	else if (access(argv[1], R_OK) != 0)
		ft_putstr_fd("Not enugh rights to access infile\n", 2);
	else
		arglist->error = 0;
	if (access(argv[4], F_OK) == 0 && access(argv[4], R_OK) != 0)
		return (ft_putstr_fd("Invalid Outfile\n", 2), free(arglist), NULL);
	else
		arglist->error = 0;
	arglist->arg1 = ft_split(argv[2], ' ');
	arglist->arg2 = ft_split(argv[3], ' ');
	return (arglist);
}

int	outpipe(t_arglist *arglist, char **envp)
{
	if (arglist->nooutfile != -1)
	{
		dup2(arglist->fd[0], STDIN_FILENO);
		dup2(arglist->outfile, STDOUT_FILENO);
		close(arglist->fd[1]);
		close(arglist->fd[0]);
		if (!arglist->path2)
		{
			ft_putstr_fd("Command not found", 2);
			ft_putchar_fd('\n', 2);
			close(arglist->outfile);
			exit(127);
		}
		execve(arglist->path2, arglist->arg2, envp);
		ft_putstr_fd("Command not found\n", 2);
		close(arglist->outfile);
		return (exit(1), 127);
	}
	else
		return (exit(1), 0);
}
