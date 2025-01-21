/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 11:09:45 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/21 15:38:23 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
/*****************************************************************************/
//           No explanation needed for this function
/*****************************************************************************/
void	handle_syntax_error(t_shell_data *shell, t_token *token)
{
	ft_putstr_fd("minishell: syntax error unexpected token", STDERR_FILENO);
	ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	shell->exit_status = 2;
}
