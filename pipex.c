/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:37:45 by djareno           #+#    #+#             */
/*   Updated: 2025/10/16 12:37:26 by djareno          ###   ########.fr       */
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
	if (arglist->infile == -1)
		arglist->noinfile = 1;
	arglist->outfile = open(argv[4], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (arglist->outfile == -1)
		arglist->nooutfile = 1;
	if (arglist->arg1 && arglist->arg1[0])
		arglist->path1 = find_cmd(arglist->arg1[0], envp);
	else
		arglist->path1 = NULL;
	if (arglist->arg2 && arglist->arg2[0])
		arglist->path2 = find_cmd(arglist->arg2[0], envp);
	else
		arglist->path2 = NULL;
}

void	inpipe(t_arglist *arglist, char **envp)
{
	if (arglist->noinfile != -1)
	{
		dup2(arglist->infile, STDIN_FILENO);
		dup2(arglist->fd[1], STDOUT_FILENO);
		close(arglist->fd[0]);
		close(arglist->fd[1]);
		if (!arglist->path1)
		{
			ft_putstr_fd("Command not found", 2);
			ft_putchar_fd('\n', 2);
			close(arglist->infile);
			exit(127);
		}
		execve(arglist->path1, arglist->arg1, envp);
		ft_putstr_fd("Command not found\n", 2);
		close(arglist->infile);
		exit(1);
	}
	else
		exit(1);
}

int	pipex(pid_t pid1, pid_t pid2, char **envp, t_arglist *arglist)
{
	int	status1;
	int	status2;
	int	exit_code;

	pipe(arglist->fd);
	pid1 = fork();
	if (pid1 == 0)
		inpipe(arglist, envp);
	pid2 = fork();
	if (pid2 == 0)
		outpipe(arglist, envp);
	close(arglist->fd[0]);
	close(arglist->fd[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	exit_code = (status2 >> 8) & 0xFF;
	return (exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_arglist	*arglist;
	pid_t		pid1;
	pid_t		pid2;
	int			numerror;

	pid1 = 0;
	pid2 = 0;
	arglist = parse_args(argc, argv);
	if (!arglist)
		exit(1);
	saveargs(arglist, argv, envp);
	numerror = arglist->error;
	if (numerror == 0)
		numerror = pipex(pid1, pid2, envp, arglist);
	freemain(arglist);
	if (numerror != 0)
		exit(numerror);
	return (0);
}
