/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvalient <mvalient@student.42urduliz.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 14:53:58 by mvalient          #+#    #+#             */
/*   Updated: 2023/03/17 14:53:58 by mvalient         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// DEBUG OR PRODUCTION (1 or 0)
# define DEBUG 0

// Required for readline compatibility
# define _XOPEN_SOURCE 700

// INCLUDES
# include "../lib/LIBFT/inc/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <signal.h>

// MESSAGES
# define PROMPT			"\033[0;92m >> $ \033[0;39m"
# define MALLOC_ERROR	"Memory could not be allocated.\n"
# define SYNTAX_ERROR	"\033[0;31mSyntax error.\033[0;39m\n"
# define VAR_ERROR		"\033[0;31mUser variable not defined.\033[0;39m\n"

// DATA STRUCTS
typedef struct s_cmd
{
	int				index;
	char			*str;
	int				is_exec;
	int				is_builtin;
	int				is_infd;
	int				is_outfd;
	int				is_pipe;
	int				infd;
	int				outfd;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_vars
{
	char			*name;
	char			*val;
	struct s_vars	*next;
	int				is_exp;
}					t_vars;

typedef struct s_data
{
	char	*input;
	char	*ex_input;
	char	*prompt;
	int		baseline_infd;
	int		baseline_outfd;
	char	**custom_envp;
	t_vars	*vars;
	t_cmd	*cmd;
}			t_data;

// FUNCTIONS:

// 01_init_data
t_data	*ft_init_data(char **envp);

// 02_check_input
int		ft_input_ok(t_data *data);

// 03_expand_input
void	ft_expand(t_data *data);

// 04_parse_cmds
void	ft_parse(t_data *data);
void	ft_last_exec(t_data *data);

// 05_setup_redir
void	ft_setup_redir(t_data *data);

// 06_exec_cmds
void	ft_exec_cmds(t_data *data);

// 07_run
int		ft_launch_piped_process(char *str, int infd, int outfd, t_data *data);
int		ft_launch_process(char *str, int infd, int outfd, t_data *data);
int		ft_launch_builtin(char *str, int infd, int outfd, t_data *data);
void	ft_heredoc(char *eof);

// 08_builtins
int		ft_echo_builtin(t_vars **env, char **cmd);
int		ft_cd_builtin_one(t_vars **env, char **cmd);
int		ft_pwd_builtin(t_vars **env, char **cmd);
int		ft_export_builtin(t_vars **env, char **cmd);
int		ft_unset_builtin(t_vars **env, char **cmd);
int		ft_env_builtin(t_vars **env, char **cmd);

// 08_builtin_utils
char	*ft_route_parser(char *route);
char	*ft_strjoin_arr(char **arr, char *del, int length);
void	*ft_free_null(void *ptr);
void	ft_set_pwd(t_vars **env, char *path);
int		ft_check_dir_permission(char *path);

// 09_signals
void	ft_signal_handler(void (*handler)
			(int signum, siginfo_t *info, void *context));
void	ft_parent_signals(int signum, siginfo_t *info, void *context);
void	ft_child_signals(int signum, siginfo_t *info, void *context);

// -------------------------------------------------------------

// 99_aux
void	ft_clear_screen(void);
int		ft_endsub(char *str, int i, char *charset);
int		ft_endredir(char *str, int i);
int		ft_inquotes(char *str, int i);
int		ft_inside(char *str, int i, char c);
int		ft_starts_with(const char *str, const char *start);
int		ft_get_expanded_len(t_data *data);

// 99_aux_cmds
void	ft_cmdadd_back(t_cmd **cmd, t_cmd *new);
t_cmd	*ft_cmdnew(char *str, int index);

// 99_aux_redir
char	*ft_filename(char *str);
void	ft_cmd_clean(t_data *data);

// 99_aux_run
char	**ft_get_args(char *arg);
char	*ft_get_path(char *cmd, t_data *data);
char	**ft_gen_envp(t_data *data);
void	ft_run_command(char *arg, t_data *data, char **envp);

// 99_aux_vars
t_vars	*ft_getenv_local(t_vars *list, char *name);
int		ft_setenv_local(t_vars *list, char *name, char *value, int overwrite);
t_vars	*ft_varsnew(char *name, char *value);
int		ft_vars_size(t_data *data); // ver si la usamos

// 99_degub
void	ft_show_vars(t_data *data);
void	ft_show_parsed(t_data *data);
void	ft_check_cmd(char *cmd_path, char **cmd);
void	ft_print_redir(char *str, t_cmd *exec, char *redir);

// 99_free_data
void	ft_free_all(t_data *data);
void	ft_freecmd(t_data *data);
void	ft_free_custom_envp(t_data *data);

// 99_aux_prompt
char	*ft_gen_prompt(t_data *data);

#endif
