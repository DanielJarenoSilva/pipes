/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:41:15 by djareno           #+#    #+#             */
/*   Updated: 2025/10/07 11:36:42 by djareno          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>

typedef struct arg_list
{
	int				infile;
	int				outfile;
	char			**arg1;
	char			**arg2;
	char			*path1;
	char			*path2;
	int				fd[2];
	int				error;
}					t_arglist;
char		*get_path(char **envp);
char		*find_cmd(char *cmd, char **envp);
int			checkcmds(char *cmd1, char *cmd2, char **envp, t_arglist *arglist);
t_arglist	*parse_args(int argc, char **argv, char **envp);
void		freemain(t_arglist *arglist);
#endif