/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 13:31:36 by pghajard          #+#    #+#             */
/*   Updated: 2025/01/22 13:33:50 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  process the filename or delimiter in the redirection
/*****************************************************************************/
//
// 🔹 Parameters:
//	   🏷  shell -> our structure
//     🏷  new_redir  ->
// 🔄 Returns   :  success status
//
/*****************************************************************************/
// 💡 Notes:
//
//     1- if the shell error flag is 4 we print an Error
//     2- if the shell error flag is not 4 we print a syntax error
//     3- we free the new redirection
//     4- we set the exit status to 2
//     5- we return 1
//
/******************************************************************************/

int	handle_missing_filename_error(t_shell_data *shell, \
t_redirection *new_redir)
{
	if (shell->error_flag == 4)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	else
		ft_putstr_fd("syntax error near unexpected token `newline'\n", \
		STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}
/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/
void	ft_putstr_fd2(const char *s, int fd)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

/*****************************************************************************/
// 💡 Notes:
//     1- no explanation needed.
//
/******************************************************************************/

int	handle_syntax_error_s(t_shell_data *shell, t_redirection *new_redir, \
const char *unexpected_token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
	STDERR_FILENO);
	ft_putstr_fd2(unexpected_token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}
/*****************************************************************************/
// 🎯 Purpose  :  skip_white spaces
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  input
//     🏷  i
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- using a while loop we iterate over the input characters as long as
//        the current character is a white space we increment the index.
//
/******************************************************************************/

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

/*****************************************************************************/
// 🎯 Purpose  :  create a new redirection and store it in new_redir linked list
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  op -> our operation
// 🔄 Returns   :  our new redirection
//
/*****************************************************************************/
// 💡 Notes:
//     1- we allocate memory for our new redirection.
//        a- if there is an error we print an error message and return NULL.
//     2- using ft_bzero() we set the new redirection elements to 0.
//     3- using ft_strcmp() we check the operation and set the type of the
//        propper redirection.
//     4- we return the new redirection.
//
/******************************************************************************/

t_redirection	*create_new_redirection(char *op)
{
	t_redirection	*new_redir;

	new_redir = malloc(sizeof(t_redirection));
	if (!new_redir)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (NULL);
	}
	ft_bzero(new_redir, sizeof(t_redirection));
	if (!ft_strcmp(op, "<"))
		new_redir->type = REDIR_INPUT;
	else if (!ft_strcmp(op, ">"))
		new_redir->type = REDIR_OUTPUT;
	else if (!ft_strcmp(op, ">>"))
		new_redir->type = REDIR_APPEND;
	else if (!ft_strcmp(op, "<<"))
		new_redir->type = REDIR_HEREDOC;

	return (new_redir);
}

