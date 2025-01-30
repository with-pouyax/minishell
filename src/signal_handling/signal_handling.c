/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 11:07:04 by pouyax            #+#    #+#             */
/*   Updated: 2025/01/23 15:32:41 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../include/minishell.h"

t_signal	g_signal = {0, 0};
/*****************************************************************************/
// 🎯 Purpose  :    set up signal for when we are in the child process and we
//					press SIGQUIT
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  sig ->  signal number
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we write "Quit (core dumped)" to the standard output.
//
/******************************************************************************/

void	child_sigquit_handler(int sig)
{
	(void)sig;
	write(1, "Quit (core dumped)\n", 19);
}

/*****************************************************************************/
//                            🚀 child_sigint_handler 🚀
/*****************************************************************************/
// 🎯 Purpose  :    set up signal for when we are in the child process and we
//					press SIGINT
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  sig ->  signal number
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we go to the next line
//     2- we set the signal status to 1.
//
/******************************************************************************/

void	child_sigint_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO, "\n", 1);
	g_signal.signal_status = 1;
}

/*****************************************************************************/
//                              🚀 sigint_hd_handler 🚀
/*****************************************************************************/
// 🎯 Purpose  :    set up signal for when we are in here document and we
//					press SIGINT
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  sig ->  signal number
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we simulate the enter key to go to the next line.
//        -  the difference between ioctl and write is that ioctl writes to
//			 the terminal driver. it means that it writes to the terminal
//			 buffer and not to the standard output, so it is like we are typing
//			 the enter key.
//     2- we move the cursor to the new line.
//     3- we replace the line with an empty string.
//     4- we set the signal status to 1.
//
/******************************************************************************/

void	sigint_hd_handler(int sig)
{
	(void)sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal.signal_status = 1;
}

/*****************************************************************************/
//                              🚀 sigint_handler 🚀
/*****************************************************************************/
// 🎯 Purpose  :    handle sigint in main process
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  sig ->  signal number
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
// 💡 Notes:
//     1- we simulate the enter key to go to the next line.
//        - the difference between ioctl and write is that ioctl writes to
//			 the terminal driver. it means that it writes to the terminal
//			 buffer and not to the standard output, so it is like we are typing
//     2- we move the cursor to the new line.
//     3- we replace the line with an empty string.
//     4- we set the signal status to 1.
//
/******************************************************************************/

void	sigint_handler(int sig)
{
	(void)sig;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	g_signal.signal_status = 1;
}

/*****************************************************************************/
//                              🚀 setup_signal_handlers 🚀
/*****************************************************************************/
// 🎯 Purpose  :    initialize signals handlers based on the type we want
/*****************************************************************************/
//
// 🔹 Parameters:
//     🏷  type ->   type represents the type of the signal handler we want
//					 to set up
//
//
// 🔄 Returns   :  void
//
/*****************************************************************************/
//
// 💡 Notes:
//     1- we set up the signal handlers based on the type we want.
//     2- 0 is for the main process.
//     3- 1 is for the time we are in here document.
//     4- 2 is for the time we are in the child process.
//	   5- I think 3 is extra and we don't need it.
/******************************************************************************/
void	setup_signal_handlers(int type)
{
	if (type == 0)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (type == 1)
	{
		signal(SIGINT, sigint_hd_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (type == 2)
	{
		signal(SIGINT, child_sigint_handler);
		signal(SIGQUIT, child_sigquit_handler);
	}
	else if (type == 3)
	{
		signal(SIGINT, child_sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
}
