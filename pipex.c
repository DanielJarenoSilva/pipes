/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:37:45 by djareno           #+#    #+#             */
/*   Updated: 2025/09/30 15:10:09 by djareno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char *find_cmd(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	char	*tmp;
	int		i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			envp[i] = envp[i] + 5;
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

int	main(int argc, char **argv, char **envp)
{
	char	*path;
	char *const args[] = {"/bin/ls", ".", NULL};
	if (argc == 0)
	{
		return (ft_printf("%s\n", "You need to introduce values"));
	}
	path = find_cmd(argv[1], envp);
	execve(path, args, envp);
	ft_printf("%s\n", path);
	return (0);
}
