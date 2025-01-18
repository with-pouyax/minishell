#include "../minishell.h"

/*****************************************************************************/
// 🎯 Purpose  :  check if the charachter is an operator
/*****************************************************************************/
//
// 🔹 Parameters:                                                             
//     🏷  c -> character                     
//
// 🔄 Returns   :  1 if it is operator, 0 if not
//
/*****************************************************************************/
// 💡 Notes:                                                                  
//     1- if the character is a pipe, <, >, >>, or << we return 1 else 0.
//       
/******************************************************************************/

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*****************************************************************************/
// 🎯 Purpose  :  check if op is valid operator or not
/*****************************************************************************/
//
// 🔹 Parameters:                                                             
//     🏷  op -> the character we recieved as operator.
//
// 🔄 Returns   :  success status.
//
/*****************************************************************************/
// 💡 Notes:                                                                  
//     1- if the op is |, <, >, >>, or << we return 1 else 0.
//       
/******************************************************************************/

int	is_valid_operator(char *op)
{
	if (!ft_strcmp(op, "|") || !ft_strcmp(op, "<")
		|| !ft_strcmp(op, ">") || !ft_strcmp(op, ">>")
		|| !ft_strcmp(op, "<<"))
		return (1);
	return (0);
}
