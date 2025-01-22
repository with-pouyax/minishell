/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_commands_utils5.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 12:56:39 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 18:12:11 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*****************************************************************************/
//      No explanation needed for this function
/*****************************************************************************/

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
//
// 🔄 Returns   : string after =
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate through the string as long as the
//        character is not a '='.
//     2- once we find the '=', using ft_substr() we return rest of the string
//        after the '='.
/******************************************************************************/

char	*parse_key(const char *str)
{
	int	len;

	len = 0;
	while (str[len] && str[len] != '=')
		len++;
	return (ft_substr(str, 0, len));
}

/*****************************************************************************/
// 🎯 Purpose  :
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  str
//     🏷  envp
//     🏷  key_len
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:
//     1-  using a while loop we iterate through the environment variables.
//         a-  if the environment variable matches the key we return the index.
//     2-  if we don't find the key we return -1.
//
/******************************************************************************/

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