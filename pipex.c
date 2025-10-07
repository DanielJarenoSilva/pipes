/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:37:45 by djareno           #+#    #+#             */
/*   Updated: 2025/10/07 11:37:35 by djareno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	freemain(t_arglist *arglist)
{
	ft_free_matrix(arglist->arg1);
	ft_free_matrix(arglist->arg2);
	free(arglist->path1);
	free(arglist->path2);
	free(arglist);
}

void	saveargs(t_arglist *arglist, char **argv, char **envp)
{
	arglist->infile = open(argv[1], O_RDONLY);
	arglist->outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	arglist->path1 = find_cmd(arglist->arg1[0], envp);
	arglist->path2 = find_cmd(arglist->arg2[0], envp);
}

void	inpipe(t_arglist *arglist, char **envp)
{
	dup2(arglist->infile, STDIN_FILENO);
	dup2(arglist->fd[1], STDOUT_FILENO);
	close(arglist->fd[0]);
	close(arglist->fd[1]);
	if (execve(arglist->path1, arglist->arg1, envp) >= 0)
		exit(1);
}

void	pipex(pid_t pid1, pid_t pid2, char **envp, t_arglist *arglist)
{
	pipe(arglist->fd);
	pid1 = fork();
	if (pid1 == 0)
		inpipe(arglist, envp);
	pid2 = fork();
	if (pid2 == 0)
	{
		dup2(arglist->fd[0], STDIN_FILENO);
		dup2(arglist->outfile, STDOUT_FILENO);
		close(arglist->fd[1]);
		close(arglist->fd[0]);
		if (execve(arglist->path2, arglist->arg2, envp) >= 0)
			exit(1);
	}
	close(arglist->fd[0]);
	close(arglist->fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_arglist	*arglist;
	pid_t		pid1;
	pid_t		pid2;
	int			numerror;

	pid1 = 0;
	pid2 = 0;
	arglist = parse_args(argc, argv, envp);
	if (!arglist)
		exit(1);
	saveargs(arglist, argv, envp);
	pipex(pid1, pid2, envp, arglist);
	numerror = arglist->error;
	freemain(arglist);
	if (numerror != 0)
		exit(numerror);
	return (0);
}
