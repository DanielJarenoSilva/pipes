/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 15:09:18 by djareno           #+#    #+#             */
/*   Updated: 2025/10/07 11:39:47 by djareno          ###   ########.fr       */
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

int	checkcmds(char *cmd1, char *cmd2, char **envp, t_arglist *arglist)
{
	cmd1 = find_cmd(cmd1, envp);
	cmd2 = find_cmd(cmd2, envp);
	if (!cmd1)
		ft_putstr_fd("Command not found\n", 2);
	if (!cmd2 && cmd1)
	{
		free(cmd1);
		free (cmd2);
		ft_putstr_fd("Command not found\n", 2);
		arglist->error = 127;
		return (1);
	}
	free(cmd1);
	free(cmd2);
	return (1);
}

t_arglist	*parse_args(int argc, char **argv, char **envp)
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
	if (checkcmds(arglist->arg1[0], arglist->arg2[0], envp, arglist) == 0)
		return (freemain(arglist), NULL);
	return (arglist);
}
