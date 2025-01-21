/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 09:45:32 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 01:23:51 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "internal_commands.h"

int	is_allowed_char(char c)
{
	return (ft_isalnum(c)
		|| c == '_'
		|| c == '-'
		|| c == '.'
		|| c == '/'
		|| c == '~'
		|| c == '$'
		|| c == '='
		|| c == ':');
}
/*****************************************************************************/
// 🎯 Purpose  :  we use this function to check if the string is a valid 
//                identifier, meaning it does not contain any special 
//                characters except for the underscore and the equals sign.
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  str
//     🏷  allow_equals
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  if the string in not exist or first character is not a letter or
//         underscore we return 0.
//     2-  we loop through the string using a while loop.
//         a-  if the character is not a letter, digit or underscore
//             I- if allow_equals is true and the character is '=' we return 1.
//             II- if allow_equals is false we return 0.
//     3-  now that we are done with the loop if allow_equals is false and
//         the string contains '=' we return 0.
//     4-  we return 1.
//
//   
/******************************************************************************/
int	is_valid_identifier(const char *str, int allow_equals)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[i]) && str[i] != '_'))
		return (0);
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
		{
			if (allow_equals && str[i] == '=')
				return (1);
			return (0);
		}
		i++;
	}
	if (!allow_equals && ft_strchr(str, '='))
		return (0);
	return (1);
}

/*****************************************************************************/
// 🎯 Purpose  :  
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  str
//     🏷  shell
//
// 🔄 Returns   :  
//
/*****************************************************************************/
// 💡 Notes:
//     1-
/******************************************************************************/

char	*parse_key(const char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (ft_substr(str, 0, len));
}

int	find_env_index(char **envp, const char *key, int key_len)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void	replace_env_var(t_shell_data *shell, int index, const char *str)
{
	free(shell->envp[index]);
	shell->envp[index] = ft_strdup(str);
	if (!shell->envp[index])
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
	}
}

char	*duplicate_new_var(const char *str, t_shell_data *shell)
{
	char	*new_var;

	new_var = ft_strdup(str);
	if (!new_var)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
	}
	return (new_var);
}

char	**allocate_new_envp(int current_size, t_shell_data *shell)
{
	char	**new_envp;

	new_envp = malloc(sizeof(char *) * (current_size + 2));
	if (!new_envp)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
	}
	return (new_envp);
}

void	copy_existing_envp(char **new_envp, char **envp, int current_size)
{
	int	i;

	i = 0;
	while (i < current_size)
	{
		new_envp[i] = envp[i];
		i++;
	}
}


void	finalize_new_envp(char **new_envp, char *new_var, int current_size)
{
	new_envp[current_size] = new_var;
	new_envp[current_size + 1] = NULL;
}

void	replace_envp(t_shell_data *shell, char **new_envp)
{
	free(shell->envp);
	shell->envp = new_envp;
}

static void	handle_new_env_var(t_shell_data *shell, const char *str, \
int current_size, char *new_var)
{
	char	**new_envp;

	(void) str;
	new_envp = allocate_new_envp(current_size, shell);
	if (!new_envp)
	{
		free(new_var);
		return ;
	}
	copy_existing_envp(new_envp, shell->envp, current_size);
	finalize_new_envp(new_envp, new_var, current_size);
	replace_envp(shell, new_envp);
	shell->exit_status = 0;
}

void	add_new_env_var(t_shell_data *shell, const char *str, int current_size)
{
	char	*new_var;

	new_var = duplicate_new_var(str, shell);
	if (!new_var)
		return ;
	handle_new_env_var(shell, str, current_size, new_var);
}
/*****************************************************************************/
// 🎯 Purpose  :  
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  str
//     🏷  shell
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  we parse the key from the string using parse_key().
//     2-  if there is an error parsing the key we print an error message
//         and return.
//     3-  we get the length of the key using ft_strlen() and store it in
//         key_len.
//     4-  we find the index of the environment variable using find_env_index()
//         a-  if the index is not -1 means the environment variable exists
//             I-  we replace the environment variable using replace_env_var().
//         b-  if the index is -1 means the environment variable does not exist
//			    I-  we add the new environment variable using add_new_env_var()
//	   5-  we free the key.
//
//   
/******************************************************************************/

void	add_to_env(t_shell_data *shell, const char *str)
{
	char	*key;
	int		key_len;
	int		index;
	int		size;

	key = parse_key(str);
	if (!key)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		shell->exit_status = 1;
		return ;
	}
	key_len = ft_strlen(key);
	index = find_env_index(shell->envp, key, key_len);
	if (index != -1)
		replace_env_var(shell, index, str);
	else
	{
		size = 0;
		while (shell->envp[size])
			size++;
		add_new_env_var(shell, str, size);
	}
	free(key);
}


int	get_name_length(const char *name)
{
	return (ft_strlen(name));
}

int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}



int	is_matching_var(const char *env_var, const char *name, int name_len)
{
	return (ft_strncmp(env_var, name, name_len) == 0 && \
	(env_var[name_len] == '=' || env_var[name_len] == '\0'));
}

int	copy_env_vars(char **new_envp, char **envp, const char *name, int name_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (envp[i])
	{
		if (!is_matching_var(envp[i], name, name_len))
		{
			new_envp[j++] = envp[i];
		}
		else
		{
			free(envp[i]);
		}
		i++;
	}
	new_envp[j] = NULL;
	return (j);
}

void	handle_remove_env(t_shell_data *shell, const char *name)
{
	int		name_len;
	int		current_size;
	char	**new_envp;

	name_len = get_name_length(name);
	current_size = count_env_vars(shell->envp);
	new_envp = allocate_new_envp(current_size, shell);
	if (!new_envp)
		return ;
	copy_env_vars(new_envp, shell->envp, name, name_len);
	free(shell->envp);
	shell->envp = new_envp;
	shell->exit_status = 0;
}

void	remove_from_env(t_shell_data *shell, const char *name)
{
	if (!name)
	{
		shell->exit_status = 0;
		return ;
	}
	handle_remove_env(shell, name);
}

/*****************************************************************************/
//             using a while loop iterate through the environment
//             variables and count the number of variables.
/*****************************************************************************/

static int	get_env_size(char **envp)
{
	int	size;

	size = 0;
	while (envp[size])
		size++;
	return (size);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell ->
//     🏷  env_size ->
//     🏷  envp ->
//
// 🔄 Returns   :  sorted envp
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for the sorted environment variables.
//     2- if there is an error allocating memory we print an error message
//        and return NULL.
//     3- we loop through the environment variables using a while loop.
//        a- we duplicate the environment variable from the original 
//        environment variables to the sorted environment variables.
//        b- if there is an error duplicating the environment variable we
//           print an error message and using a while loop we free the
//           environment variables array backwards and return NULL.
//        c- we free the sorted_envp array and return NULL.
//     4- now that the loop is done and we have duplicated all the environment
//        variables we set the last element of the sorted environment variables
//        to NULL.
//     5- we return the sorted environment variables.
// 
//     we need to duplicate the environment variables because we need to sort
//     them and we don't want to modify the original environment variables.
//     we need to sort the environment variables because we need to print them
//     in a sorted order.
/******************************************************************************/

static char	**duplicate_envp(char **envp, int env_size, t_shell_data *shell)
{
	char	**sorted_envp;
	int		i;

	sorted_envp = malloc(sizeof(char *) * (env_size + 1));
	if (!sorted_envp)
	{
		ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
		return (shell->exit_status = 1, NULL);
	}
	i = 0;
	while (i < env_size)
	{
		sorted_envp[i] = ft_strdup(envp[i]);
		if (!sorted_envp[i])
		{
			ft_putstr_fd("minishell: malloc failed\n", STDERR_FILENO);
			while (--i >= 0)
				free(sorted_envp[i]);
			free(sorted_envp);
			return (shell->exit_status = 1, NULL);
		}
		i++;
	}
	sorted_envp[env_size] = NULL;
	return (sorted_envp);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  envp ->
//     🏷  env_size ->
//
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  using a while loop we iterate through the environment variables.
//         a-  using a nested while loop we iterate through the environment
//             variables starting second element.
//         b- using ft_strcmp() we compare the two environment variables, if
//            they ft_strcmp() returns a positive number means the two strings
//            are not in the correct order because the first string is greater
//            than the second string so we swap the two strings.
//            - here i am actually sorting the environment variables in
//              sorting algorithm called bubble sort.
/******************************************************************************/

static void	sort_envp(char **envp, int env_size)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < env_size - 1)
	{
		j = i + 1;
		while (j < env_size)
		{
			if (ft_strcmp(envp[i], envp[j]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[j];
				envp[j] = temp;
			}
			j++;
		}
		i++;
	}
}

/*****************************************************************************/
//  using a while loop we iterate through the environment variables and print
//  them using printf() with the format "declare -x %s\n".
//  we used declare -x to print the environment variables in the format that
//  bash uses.
/******************************************************************************/

static void	print_and_free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("declare -x %s\n", envp[i]);
		free(envp[i]);
		i++;
	}
	free(envp);
}
/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  shell -> our structure
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1-  we get the size of the environment using get_env_size().
//     2-  we duplicate the environment using duplicate_envp().
//        a- if there is an error making the duplicate we return.
//     3-  we sort the environment using sort_envp().
//     4-  we print the environment using print_and_free_envp().
//     5-  we set the exit status to 0.
/******************************************************************************/

void	print_sorted_env(t_shell_data *shell)
{
	char	**sorted_envp;
	int		env_size;

	env_size = get_env_size(shell->envp);
	sorted_envp = duplicate_envp(shell->envp, env_size, shell);
	if (!sorted_envp)
		return ;
	sort_envp(sorted_envp, env_size);
	print_and_free_envp(sorted_envp);
	shell->exit_status = 0;
}
