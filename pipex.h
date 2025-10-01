/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djareno <djareno@student.42madrid.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 11:41:15 by djareno           #+#    #+#             */
/*   Updated: 2025/10/01 13:09:02 by djareno          ###   ########.fr       */
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
}					t_arglist;

#endif