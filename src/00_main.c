/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalient <mvalient@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:53:58 by mvalient          #+#    #+#             */
/*   Updated: 2023/03/17 14:53:58 by mvalient         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_data	*g_data;

static void	ft_process_input(void)
{
	ft_expand(g_data);
	ft_parse(g_data);
	ft_last_exec(g_data);
	if (DEBUG == 1)
		ft_show_parsed(g_data);
	ft_setup_redir(g_data);
	if (DEBUG == 1)
		ft_show_parsed(g_data);
	ft_exec_cmds(g_data);
	free (g_data->ex_input);
	g_data->ex_input = NULL;
	ft_freecmd(g_data);
}

static void	ft_prompt(void)
{
	if (g_data->prompt != NULL)
		free (g_data->prompt);
	g_data->prompt = ft_gen_prompt(g_data);
	if (g_data->prompt != NULL)
		g_data->input = readline(g_data->prompt);
	else
		g_data->input = readline(PROMPT);
	if (!g_data->input)
		exit(EXIT_SUCCESS);
	if (g_data->input && ft_strlen(g_data->input) > 0)
	{
		add_history(g_data->input);
		rl_replace_line("", 0);
		rl_redisplay();
	}
	if (ft_input_ok(g_data))
		ft_process_input();
	if (g_data->input != NULL)
		free (g_data->input);
	g_data->input = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
		ft_exit_w_error(SYNTAX_ERROR);
	g_data = ft_init_data(envp);
	if (DEBUG == 1)
		ft_show_vars(g_data);
	else
		ft_clear_screen();
	rl_initialize();
	using_history();
	ft_signal_handler(ft_parent_signals);
	while (1)
		ft_prompt();
}
