/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   06_exec_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalient <mvalient@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:53:58 by mvalient          #+#    #+#             */
/*   Updated: 2023/03/17 14:53:58 by mvalient         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static int	ft_piped_exec(t_cmd *tmp, t_data *data)
{
	int		status;

	status = 1;
	if (tmp->is_builtin)
	{
		tmp->outfd = open(".tempfd", O_WRONLY | O_CREAT | O_TRUNC, 0666);
		status = ft_launch_builtin(tmp->str, tmp->infd, tmp->outfd, data);
		tmp->next->next->infd = open(".tempfd", O_RDONLY);
	}
	else if (tmp->is_exec)
	{
		if (tmp->outfd != STDOUT_FILENO)
		{
			tmp->next->next->infd = open(".bank",
					O_WRONLY | O_CREAT | O_TRUNC, 0666);
			status = ft_launch_process(tmp->str, tmp->infd, tmp->outfd, data);
			tmp->next->next->infd = open(".bank", O_RDONLY);
		}
		else
			status = ft_launch_piped_process(tmp->str, tmp->infd,
					tmp->outfd, data);
	}
	return (status);
}

static int	ft_simple_exec(t_cmd *tmp, t_data *data)
{
	int		status;

	status = 1;
	if (tmp->is_builtin)
		status = ft_launch_builtin(tmp->str, tmp->infd, tmp->outfd, data);
	else if (tmp->is_exec)
		status = ft_launch_process(tmp->str, tmp->infd, tmp->outfd, data);
	return (status);
}

void	ft_exec_cmds(t_data *data)
{
	t_cmd	*tmp;
	int		status;
	char	*status_str;

	status = 1;
	tmp = data->cmd;
	while (tmp)
	{
		if (tmp->next && tmp->next->is_pipe && tmp->next->next)
			status = ft_piped_exec(tmp, data);
		else
			status = ft_simple_exec(tmp, data);
		status_str = ft_itoa(status);
		ft_setenv_local(data->vars, "?", status_str, 1);
		free(status_str);
		tmp = tmp->next;
	}
}
