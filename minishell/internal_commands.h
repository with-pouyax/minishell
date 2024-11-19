// internal_commands.h
#ifndef INTERNAL_COMMANDS_H
# define INTERNAL_COMMANDS_H

# include "minishell.h"

/* Internal Command Functions */
int		ft_echo(t_shell_data *shell, t_command *cmd);
int		ft_cd(t_shell_data *shell, t_command *cmd);
int		ft_pwd(t_shell_data *shell, t_command *cmd);
int		ft_export(t_shell_data *shell, t_command *cmd);
int		ft_unset(t_shell_data *shell, t_command *cmd);
int		ft_env(t_shell_data *shell, t_command *cmd);
int		ft_exit_shell(t_shell_data *shell, t_command *cmd); // Renamed to avoid conflict with exit()

/* Helper Functions */
int		is_numeric(const char *str);
int		is_valid_identifier(const char *str);
void	add_to_env(t_shell_data *shell, const char *str);
void	remove_from_env(const char *name);
void	print_sorted_env(t_shell_data *shell);

#endif
