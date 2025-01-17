#include "../minishell.h"

int	**init_pipes(int cmds_nb)
{
	int	**pipes;
	int	i;

	i = 0;
	if (cmds_nb == 1)
		return (NULL);
	pipes = malloc(sizeof(int *) * (cmds_nb - 1));
	if (!pipes)
		return (NULL);
	while (i < (cmds_nb - 1))
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (NULL);
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe error");
			exit(EXIT_FAILURE);
		}		i++;
	}
	return (pipes);
}

void	close_pipes_after_execution(t_shell_data *shell, int cmds_index)
{
	int	**pipes;
	int	cmds_nb;

	pipes = shell->pipes;
	cmds_nb = shell->cmds_nb;
	if (cmds_index != 0)
		close(pipes[cmds_index - 1][0]);
	if (cmds_index != cmds_nb - 1)
		close(pipes[cmds_index][1]);
}

void	close_all_pipes(int **pipes, int nb_cmds)
{
	int	i;

	i = 0;
	while (i < nb_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);

}
#include "../minishell.h"

void	check_input(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
	if (has_redirs(redir, REDIR_INPUT))
		return ;
	if (cmds_index != 0)
	{
		if (dup2(shell->pipes[cmds_index - 1][0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed for input pipe");
			shell->exit_status = EXIT_FAILURE;
			return ;
		}
	}
}

void	check_output(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
	(void)redir;
	if (has_redirs(redir, REDIR_OUTPUT) || has_redirs(redir, REDIR_APPEND))
		return ;
	if (cmds_index != shell->cmds_nb - 1)
	{
		if (dup2(shell->pipes[cmds_index][1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed for input pipe");
			shell->exit_status = EXIT_FAILURE;
			return ;
		}
	}
}

/*
 in open_all_files(redirs) processes:
if multiple input redirections exist, the last file is the one that
remains open because the file descriptor is overwritten.
cmd < input1 < input2 will read input only from input2.
cmd > output1 > output2 will write output only to output2.
The first process:
        Doesn't set up piping for STDIN since it has no previous process.
The middle processes:
        Use the pipe from the previous process for STDIN.
        Write their STDOUT into their own pipe for the next process to read.
The last process:
        Doesn't set up piping for STDOUT since there's no next process
       	to send data
diagram for a pipeline with three processes (A | B | C):
Process A:
    Reads input normally (STDIN).
    (when it is first cmd so not gonna do anything in check_input_redir();)
    Writes output to pipe[0][1].
Process B:
    Reads input from pipe[0][0].
    Writes output to pipe[1][1].
Process C:
    Reads input from pipe[1][0].
    Writes output normally (STDOUT).
    (when it is last cmd so not gonna do anything in check_output_redir();)
*/
void	set_redirection(t_shell_data *shell, t_redirection *redir)
{
	int	exit_code;

	while (redir)
	{
		if (redir->type == REDIR_HEREDOC)
		{
			handle_heredoc(shell, redir);
			if (shell->exit_status == EXIT_FAILURE)
				return ;
		}
		else
		{
			exit_code = open_all_files(shell, redir);
			if (exit_code == EXIT_FAILURE)
				return ;
		}
		redir = redir->next;
	}
	shell->exit_status = EXIT_SUCCESS;
}

void	set_pipes(t_shell_data *shell, t_redirection *redir, int cmds_index)
{
	check_input(shell, redir, cmds_index);
	check_output(shell, redir, cmds_index);
}
#include "../minishell.h"
/*
  // Create a pipe to pass heredoc input to the next command
    if (pipe(pipe_fd) == -1) {
 // Redirect STDIN to the heredoc file so the content can be used by the 
    next command
    if (dup2(heredoc_fd, STDIN_FILENO) == -1)
 // Redirect STDOUT to the pipe so the content can be piped to the next process
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
*/
void	handle_heredoc(t_shell_data *shell, t_redirection *redir)
{
	int	heredoc_fd;

	heredoc_fd = open(redir->heredoc_file, O_RDONLY);
	if (heredoc_fd == -1)
	{
		perror("Failed to open heredoc file");
		shell->exit_status = EXIT_FAILURE;
		return ;
	}
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("Failed to redirect STDIN to heredoc");
		close(heredoc_fd);
		shell->exit_status = EXIT_FAILURE;
		return ;
	}
	close(heredoc_fd);
}

/*
unlink(redir->heredoc_file); // Remove the temporary file
*/
void	cleanup_heredocs(t_redirection *redir)
{
	while (redir)
	{
		if (redir->type == REDIR_HEREDOC && redir->heredoc_file)
		{
			unlink(redir->heredoc_file);
		}
		redir = redir->next;
	}
}
#include "../minishell.h"

// Function to create directories in the path if they don't exist
#include <sys/stat.h>
#include <libgen.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>



/*
 If -2 then no file has been opened otherwise it means, another file is about 
 to be opened and we need to close the previous one
 example: command < file1 < file2 > file3 > file4
   The last input redirection (< file2) will be the one that is actually used.
   The last output redirection (> file4) will be the one that is actually used.
*/
int	open_input_file(t_shell_data *shell, t_redirection *redir, int fd_in_prev)
{
	int	new_fd;

	if (fd_in_prev != -2)
		close(fd_in_prev);

	new_fd = open(redir->filename, O_RDONLY, 0);
	if (new_fd == -1)
	{
		if (!shell->last_error_file)
		{
			shell->last_error_file = redir->filename;
			shell->exit_status = errno;
		}
	}
	else
	{
		dup2(new_fd, STDIN_FILENO);
		close(new_fd);
	}
	return (new_fd);
}

int	open_output_file(t_shell_data *shell, t_redirection *redir, int fd_out_prev)
{
	int	new_fd;

	if (fd_out_prev != -2)
		close(fd_out_prev);
	new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (new_fd == -1)
	{
		shell->last_error_file = redir->filename;
		shell->exit_status = errno;
	}
	else
	{
		dup2(new_fd, STDOUT_FILENO);
		close(new_fd);
	}
	return (new_fd);
}


int	open_append_file(t_shell_data *shell, t_redirection *redir, int fd_out_prev)
{
	int	new_fd;

	if (fd_out_prev != -2)
		close(fd_out_prev);
	new_fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (new_fd == -1)
	{
		shell->last_error_file = redir->filename;
		shell->exit_status = errno;
	}
	{
		dup2(new_fd, STDOUT_FILENO);
		close(new_fd);
	}
	return (new_fd);
}
#include "../minishell.h"

/*
Restore the original stdin and stdout
Close the saved file descriptors

*/
// void	restore_org_in_out(int saved_stdin, int saved_stdout)
// {
// 	dup2(saved_stdin, STDIN_FILENO);
// 	dup2(saved_stdout, STDOUT_FILENO);
// 	close(saved_stdin);
// 	close(saved_stdout);
// }

/*
set_redir() :
    handle the redirection operators in a program. 
    Redirections determine how input and output are handled for 
    commands, such as reading from a file or writing to a file.
    -- fd_i and fd_o :
    represent the file descriptors for input (<) and output (> or >>) files.
    They are initialized to -2, a special value indicating no file is open yet.
*/
void	exec_cmd(t_shell_data *shell, t_command *cmds, int index)
{
	shell->saved_stdin = dup(STDIN_FILENO);
	shell->saved_stdout = dup(STDOUT_FILENO);
	if (shell->saved_stdin == -1 || shell->saved_stdout == -1)
	{
		perror("dup failed");
		return ;
	}
	set_redirection(shell, cmds->redirections);
	if (shell->exit_status == EXIT_SUCCESS)
	{
		set_pipes(shell, cmds->redirections, index);
		cleanup_heredocs(cmds->redirections);
		forking(shell, cmds);
	}
	// restore_org_in_out(shell->saved_stdin, shell->saved_stdout);
	dup2(shell->saved_stdin, STDIN_FILENO);
	dup2(shell->saved_stdout, STDOUT_FILENO);
	close(shell->saved_stdin);
	close(shell->saved_stdout);

}

void	execution(t_shell_data *shell)
{
	t_command	*cmd;
	int			i;

	i = 0;
	cmd = shell->commands;
	shell->pipes = init_pipes(shell->cmds_nb);
	while (i < shell->cmds_nb)
	{
		exec_cmd(shell, cmd, i);
		cmd = cmd->next;
		i++;
	}
	close_all_pipes(shell->pipes, shell->cmds_nb);
	execute_parent(shell);
}
#include "../minishell.h"

/*
		Save the exit status of the last child process:
(WIFEXITED(status))    --->  Normal termination
(WIFSIGNALED(status))  ---> Terminated by a signal
shell->exit_status = 1 ---> Fallback for unexpected cases
*/
void	execute_parent(t_shell_data *shell)
{
	t_pid_node	*current;
	pid_t		pid;
	int			status;

	current = shell->pid_list;
	while (current != NULL)
	{
		pid = current->pid;
		if (waitpid(pid, &status, 0) == -1)
			perror("waitpid failed");
		else
		{
			if (WIFEXITED(status))
				shell->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_status = 128 + WTERMSIG(status);
			else
				shell->exit_status = 1;
		}
		current = current->next;
	}
	clear_pid_list(shell);
}

void	clear_pid_list(t_shell_data *shell)
{
	t_pid_node	*current;
	t_pid_node	*next_node;

	current = shell->pid_list;
	while (current != NULL)
	{
		next_node = current->next;
		free(current);
		current = next_node;
	}
	shell->pid_list = NULL;
}
#include "../minishell.h"

int	has_redirs(t_redirection *redir, t_redirection_type type )
{
	while (redir)
	{
		if (redir->type == type)
			return (1);
		redir = redir->next;
	}
	return (0);
}

static int	handle_redir(t_shell_data *shell, t_redirection *redir,
				int *fd_input, int *fd_output)
{
	if (redir->type == REDIR_INPUT)
		*fd_input = open_input_file(shell, redir, *fd_input);
	else if (redir->type == REDIR_OUTPUT)
		*fd_output = open_output_file(shell, redir, *fd_output);
	else if (redir->type == REDIR_APPEND)
		*fd_output = open_append_file(shell, redir, *fd_output);
	if (*fd_input == -1 || *fd_output == -1)
	{
		write_error(shell->last_error_file, strerror(shell->exit_status));
		shell->exit_status = 1;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

static int	finalize_redir(t_shell_data *shell, int fd_input, int fd_output)
{
	if (shell->exit_status != 0 && shell->last_error_file)
		write_error(shell->last_error_file, strerror(shell->exit_status));
	if (fd_input == -1 || fd_output == -1)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

// use of fds here : to check If any of the open_file() returned -1,
// it means there was an error
// Opens all files. Only the last of its type is left open, others are closed.
int	open_all_files(t_shell_data *shell, t_redirection *redir)
{
	int	fd_input;
	int	fd_output;

	fd_input = -2;
	fd_output = -2;
	shell->last_error_file = NULL;
	while (redir)
	{
		if (handle_redir(shell, redir, &fd_input, &fd_output) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		redir = redir->next;
	}
	return (finalize_redir(shell, fd_input, fd_output));
}
#include "../minishell.h"

int	check_syntax_error(t_shell_data *shell, char *error_message)
{
	if (error_message)
	{
		ft_putstr_fd(error_message, STDERR_FILENO);
		free(shell->input);
		free(shell->full_input);
		shell->input = NULL;
		shell->full_input = NULL;
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}
#include "../minishell.h"

static void	free_envp_copy(char **new_envp, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		free(new_envp[j]);
		j++;
	}
	free(new_envp);
}

/******************************************************************************/  
//                              🚀 copy_envp 🚀                                         
/******************************************************************************/
// 🎯 Purpose  :   copy envp
/******************************************************************************/

// 🔹 Parameters:                                                             
//     🏷  envp  ->  envp is an array of strings that represent the 
//		   environment variables in the form of "key=value"                                          


// 🔄 Returns   :  new_envp

/******************************************************************************/

// 💡 Notes:                                                                  
//     1- if envp is NULL (no environment variables) or envp[0] is NULL (no 
//	   environment variables) we exit with failure.
//	   2- we count the number of environment variables.
//	   3- we allocate memory for the new_envp array with the size of the
//	   number of environment variables + 1. +1 for the NULL terminator.
//	   4- we copy the environment variables to the new_envp array using a
//	   while loop.
//	   5- if the copy fails we free the memory allocated for the new_envp array
//	   using the free_envp_copy function and return NULL.
//	   6- we NULL terminate the new_envp array.
//	   7- we return the new_envp array.                
//     
/******************************************************************************/

char	**copy_envp(char **envp) 
{
	int	i;
	int	count;
	char	**new_envp;

	count = 0;
	i = 0;
	if (!envp || !envp[0])
		exit(EXIT_FAILURE);
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			free_envp_copy(new_envp, i);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}
#include "../minishell.h"

/*
if (input[i] == '\'' && !in_double_quote)-> if single q and not in double q
in_single_quote = !in_single_quote; -> reverse the value of in_single_q
else if (input[i] == '\"' && !in_single_quote)-> if double q and not in single q
in_double_quote = !in_double_quote;-> reverse the value of in_double_q
return (in_single_quote || in_double_quote);-> Return 1 if any of the q 
						are 1, 0 otherwise.
*/
int	check_unclosed_quotes(char *input)
{
	int	in_single_quote;
	int	in_double_quote;
	int	i;

	in_single_quote = 0;
	in_double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (input[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}

/*
if (c == '\'' && !(*in_double_quote))-> If the char is a single q
and we are not in a double q
*in_single_quote = !(*in_single_quote)-> reverse the value of in_single_q
else if (c == '\"' && !(*in_single_quote))->If the char is a double q and
we are not in a single q
*in_double_quote = !(*in_double_quote)-> reverse the value of in_double_q
*/
void	update_quote_flags(char c, int *in_single_quote, int *in_double_quote)
{
	if (c == '\'' && !(*in_double_quote))
		*in_single_quote = !(*in_single_quote);
	else if (c == '\"' && !(*in_single_quote))
		*in_double_quote = !(*in_double_quote);
}
#include "../../minishell.h"

int	initialize_word(char **word)
{
    *word = ft_strdup("");
    if (!*word)
        return (1);
    return (0);
}

int	should_break(const t_shell_data *shell, char c)
{
    if (!shell->in_single_quote && !shell->in_double_quote)
    {
        if (ft_isspace(c) || is_operator_char(c))
            return (1);
    }
    return (0);
}

void	toggle_quotes(t_shell_data *shell, char c)
{
    if (c == '\'' && !shell->in_double_quote)
        shell->in_single_quote = !shell->in_single_quote;
    else if (c == '\"' && !shell->in_single_quote)
        shell->in_double_quote = !shell->in_double_quote;
}

int	add_current_char(t_shell_data *shell, char c, char **word)
{
    int	ret;

    ret = add_char_to_token(word, c);
    if (ret)
    {
        free(*word);
        return (1);
    }
    shell->prev_char = c;
    return (0);
}

int	handle_unclosed_quotes(t_shell_data *shell, char **word)
{
    if (shell->in_single_quote || shell->in_double_quote)
    {
        ft_putstr_fd("minishell: syntax error: unclosed quote\n", \
        STDERR_FILENO);
        free(*word);
        return (1);
    }
    return (0);
}

int	finalize_word(char **word)
{
    if (ft_strlen(*word) == 0)
    {
        free(*word);
        *word = NULL;
    }
    return (0);
}

int	process_character(t_shell_data *shell, char c, char **word)
{
    if ((c == '\'' && !shell->in_double_quote) || (c == '\"' && \
    !shell->in_single_quote))
    {
        toggle_quotes(shell, c);
    }
    if (add_current_char(shell, c, word))
        return (1);
    return (0);
}

int	collect_word(char *input, int *i, char **word, \
t_shell_data *shell)
{
    int	ret;

    if (!*word)
    {
        if (initialize_word(word))
        {
            shell->error_flag = 4;
            return (1);
        }
    }
    while (input[*i])
    {
        if (should_break(shell, input[*i]))
            break;
        ret = process_character(shell, input[*i], word);
        if (ret)
            return (1);
        (*i)++;
    }
    if (handle_unclosed_quotes(shell, word))
        return (1);
    return (finalize_word(word));
}

int	free_word_and_return(char *word, int ret)
{
    free(word);
    return (ret);
}

int	free_original_and_expanded_and_return(char *original_word, \
char *expanded_word, int ret)
{
    free(original_word);
    if (expanded_word)
        free(expanded_word);
    return (ret);
}


int	save_and_expand_word(t_shell_data *shell, char *word, \
char **expanded_word, char **original_word)
{
    *original_word = ft_strdup(word);
    if (!*original_word)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (1);
    }
    *expanded_word = expand_variables_in_token(shell, word);
    if (!*expanded_word)
    {
        free(*original_word);
        return (1);
    }
    return (0);
}


int	add_token_to_command(t_command *cmd, char *word)
{
    if (add_token(word, &cmd->token_list, &(cmd->token_index), 0))
        return (1);
    return (0);
}

int	set_original_value(t_command *cmd, char *original_word)
{
    t_token *last_token;

    last_token = cmd->token_list;
    while (last_token->next)
        last_token = last_token->next;
    last_token->original_value = original_word;
    return (0);
}

int	collect_and_expand_word(t_shell_data *shell, char *input, \
int *i, char **expanded_word)
{
	char	*word;

	word = ft_strdup("");
	if (!word || collect_word(input, i, &word, shell)) 
    {
        ft_putstr_fd("minishell: memory allocation error\n", \
        STDERR_FILENO);
		return (free_word_and_return(word, 1));
    }
	if (save_and_expand_word(shell, word, expanded_word, \
    &shell->original_word))
		return (free_word_and_return(word, 1));
	free(word);
	return (0);
}

void	free_expanded_word_arr(char **expanded_word_arr, int j)
{
	while (j > 0)
		free(expanded_word_arr[--j]);
	free(expanded_word_arr);
}

int	handle_token_addition_failure(char **expanded_word_arr, int j, \
char *expanded_word, char *original_word)
{
	free(expanded_word_arr[j]);
	free_expanded_word_arr(expanded_word_arr, j);
	free(expanded_word);
	free(original_word);
	return (1);
}

int	add_tokens_to_command(t_command *cmd, char **expanded_word_arr)
{
	int	j;

	j = 0;
	while (expanded_word_arr[j])
	{
		if (add_token_to_command(cmd, expanded_word_arr[j]))
			return (handle_token_addition_failure(expanded_word_arr, j, \
            expanded_word_arr[j], NULL));
		j++;
	}
	return (0);
}

char	**split_expanded_word(char *expanded_word)
{
	char	**expanded_word_arr;

	expanded_word_arr = ft_split(expanded_word, ' ');
    if (!expanded_word_arr)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
    }
    return (expanded_word_arr);
}
void	free_all_resources(char **expanded_word_arr, char *expanded_word, \
char *original_word)
{
	free(expanded_word_arr);
	free(expanded_word);
	free(original_word);
}

int	handle_expanded(t_shell_data *shell, t_command *cmd, \
char *expanded_word, char *original_word)
{
	char	**expanded_word_arr;

	expanded_word_arr = split_expanded_word(expanded_word);
	if (!expanded_word_arr)
	{
		free(expanded_word);
		free(original_word);
		return (1);
	}
	if (add_tokens_to_command(cmd, expanded_word_arr))
		return (1);
	free_all_resources(expanded_word_arr, expanded_word, original_word);
	shell->expanded = 0;
    shell->double_quoted = 0;
	return (0);
}

int	handle_non_expanded(t_shell_data *shell, t_command *cmd, \
char *expanded_word, char *original_word)
{
    (void)shell;
	if (add_token_to_command(cmd, expanded_word))
		return (free_original_and_expanded_and_return(original_word, \
        expanded_word, 1));
	if (set_original_value(cmd, original_word))
		return (1);
	return (0);
}

int	process_word(t_shell_data *shell, char *input, int *i, t_command *cmd)
{
	char	*expanded_word;
	char	*original_word;

    
	if (collect_and_expand_word(shell, input, i, &expanded_word))
		return (1);
    original_word = shell->original_word;
	if (shell->expanded && !shell->double_quoted)
	{
		if (handle_expanded(shell, cmd, expanded_word, original_word))
			return (1);
	}
	else
	{
		if (handle_non_expanded(shell, cmd, expanded_word, original_word))
			return (1);
	}
	return (0);
}
#include "../../minishell.h"

void	trim_trailing_spaces(char *cmd_str)
{
	int	len;

	if (!cmd_str)
		return ;
	len = ft_strlen(cmd_str);
	while (len > 0 && ft_isspace(cmd_str[len - 1]))
		cmd_str[--len] = '\0';
}

int	handle_command_creation_error(t_shell_data *shell,
				char *cmd_str, t_command *cmd)
{
	free(cmd_str);
	free(cmd);
	shell->error_flag = 3;
	return (1);
}

int process_input_segment(t_shell_data *shell, int *i, int *cmd_index,
                          t_command **last_cmd)
{
    int start;
    char *cmd_str;
    t_command *cmd;

    start = *i;
    (void)start;
    *i = extract_command_string(shell->input, *i);
    if (*i == -1)                                                
        return (shell->error_flag = 1, 1);
    cmd_str = ft_substr(shell->input, start, *i - start);
    if (!cmd_str)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (shell->error_flag = 2, 1);
    }
    trim_trailing_spaces(cmd_str);
    cmd = create_command(shell, cmd_str, (*cmd_index)++);        
    if (!cmd || tokenize_command(shell, cmd))
        return (handle_command_creation_error(shell, cmd_str, cmd));
    add_command_to_list(shell, last_cmd, cmd);
    if (shell->input[*i] == '|')
        (*i)++;
    return (0);
}
#include "../../minishell.h"
#include "../../minishell.h"

void	parse_tokens(t_shell_data *shell)
{
	t_command	*cmd;
	t_token		*token;

	cmd = shell->commands;
	while (cmd)
	{
		token = cmd->token_list;
		while (token)
		{
			if (token->index == 0 && !token->is_operator)
			{
				token->is_command = 1;
				if (is_internal_command(token->value))
					token->is_int = 1;
			}
			else if (ft_strncmp(token->value, "-", 1) == 0)
				token->is_flag = 1;
			token = token->next;
		}
		cmd = cmd->next;
	}
}
#include "../../minishell.h"

int tokenize_command(t_shell_data *shell, t_command *cmd)
{
    int i;

    i = 0;
    while (cmd->command_string[i])
    {
        skip_cmd_spaces(cmd->command_string, &i);                           
        if (cmd->command_string[i])
        {
            if (is_operator_char(cmd->command_string[i]))
            {
                if (process_operator(shell, &i, cmd))
                    return (tokenize_command_error(cmd));
            }
            else
            {
                if (process_word(shell, cmd->command_string, &i, cmd))
                    return (tokenize_command_error(cmd));
            }
        }
    }
    return (0);
}

void	skip_cmd_spaces(char *str, int *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}

int	handle_initial_pipe(char *input, int *i)
{
	if (input[*i] == '|' && (input[*i + 1] == '<' || input[*i + 1] == '>') && \
	!ft_isspace(input[*i + 1]))
	{
		(*i)++;
	}
	return (0);
}

char *collect_operator(t_command *cmd, int *i)
{
    char *op;
    int start;

    start = *i;
	(void)start;
    if (is_operator_char(cmd->command_string[*i]))
    {
        (*i)++;
        if ((cmd->command_string[*i - 1] == '<' || cmd->command_string[*i - 1] == '>') &&
            cmd->command_string[*i] == cmd->command_string[*i - 1])
            (*i)++;
    }
    op = ft_substr(cmd->command_string, start, *i - start);
    if (!op)
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
	return op;
}


int	handle_redirection_operator(t_shell_data *shell, char *op, \
t_command *cmd, int *i)
{
	cmd->current_op = op;
	if (handle_redirection(shell, cmd->command_string, i, cmd))
		return (1);
	return (0);
}

int	handle_pipe_operator(char *op, t_command *cmd)
{
	(void)cmd;
    free(op);
    return (0);
}


int	handle_invalid_operator(t_shell_data *shell, char *op)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
	STDERR_FILENO);
	ft_putstr_fd(op, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	shell->exit_status = 2;
	free(op);
	return (1);
}

int	process_operator(t_shell_data *shell, int *i, t_command *cmd)
{
	char	*op;
	int		ret;

	op = collect_operator(cmd, i);               
	if (!op)
		return (1);
	if (is_valid_operator(op))
	{
		if (is_redirection_operator(op))
			ret = handle_redirection_operator(shell, op, cmd, i);
		else
			ret = handle_pipe_operator(op, cmd);
		if (ret)													   
			return (1);
	}
	else 
	{
		ret = handle_invalid_operator(shell, op);
		return (ret);
	}
	return (0);
}
# include "../../minishell.h"

void	add_command_to_list(t_shell_data *shell, t_command **last_cmd, \
t_command *cmd)
{
	if (!shell->commands)
		shell->commands = cmd;
	else
		(*last_cmd)->next = cmd;
	*last_cmd = cmd;
}

t_command	*create_command(t_shell_data *shell, char *cmd_str, int index)
{
    t_command *cmd;

    (void)shell;
    cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (NULL);
    }
    cmd->command_string = cmd_str;
    cmd->index = index;
    cmd->is_recalled = 0;
    cmd->token_list = NULL;
    cmd->redirections = NULL;
    cmd->token_index = 0;
    cmd->next = NULL;
    return (cmd);
}

int extract_command_string(char *input, int i)
{
    while (input[i] && input[i] != '|')
	{
		if (input[i] == '\'' || input[i] == '\"')
			i = skip_quotes(input, i);
		else
			i++;
	}
    if (input[i] == '|' && input[i + 1] == '|')
        return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO), -1);
    if (input[i] && input[i + 1] == ' ')
    {
        i++;
        while (input[i] && input[i] == ' ')
            i++;
        if (input[i] == '|')
            return (ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO), -1);
    }    
    return (i);
}
#include "../../minishell.h"

void handle_tokenization_error(t_shell_data *shell, int error_flag)
{
    if (error_flag == 1)
        shell->exit_status = 2;
    else if (error_flag == 2)
    {
        ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
        cleanup(shell);
        exit(EXIT_FAILURE);
    }
}

void	tokenize_input_error(t_shell_data *shell, int error_flag)
{
	free_commands(shell);
	shell->commands = NULL;
	shell->error_flag = error_flag;
}
#include "../../minishell.h"

void	count_commands_and_pipes(t_shell_data *shell)
{
	int			cmd_count;
	t_command	*cmd;

	cmd_count = 0;
	cmd = shell->commands;
	while (cmd)
	{
		cmd_count++;
		cmd = cmd->next;
	}
	shell->cmds_nb = cmd_count;
	shell->pipe_nb = cmd_count - 1;
}

void	handle_tokenization_failure(t_shell_data *shell)
{
	free_commands(shell);
	shell->commands = NULL;
	handle_tokenization_error(shell, shell->error_flag);
}

int	split_cmd_tokenize(t_shell_data *shell)
{
	int		i;
	int		cmd_index;
	t_command	*last_cmd;

	i = 0;
	cmd_index = 0;
	last_cmd = NULL;
	shell->error_flag = 0;
	if (!shell->input)
		return (1);
	while (shell->input[i])
	{
		skip_spaces(shell, &i);
		if (process_input_segment(shell, &i, &cmd_index, &last_cmd) > 0)
			return (1);
	}
	if (shell->error_flag)
		handle_tokenization_failure(shell);
	else
		count_commands_and_pipes(shell);
	return (0);
}
#include "../../minishell.h"

void add_redirection(t_redirection **redirections, t_redirection *new_redir)
{
    t_redirection *current;

    if (!*redirections)
        *redirections = new_redir;
    else
    {
        current = *redirections;
        while (current->next)
            current = current->next;
        current->next = new_redir;
        new_redir->redir_number = current->redir_number + 1; 
	}
}

int starts_with_operator_char(char c)
{
    return (c == '<' || c == '>' || c == '|');
}


int	handle_heredoc_redirection(t_shell_data *shell, t_redirection *new_redir,
							char *filename_or_delimiter)
{
	new_redir->delimiter = filename_or_delimiter;
	if (read_heredoc_content(shell, new_redir))
	{
		free(new_redir->delimiter);
		free(new_redir);
		return (1);
	}
	return (0);
}
char *rm_quotes(char *word)
{
    int i;
    int j;

    i = 0;
    j = 0;
    while (word[i] != '\0')
    {
        if (word[i] != '\'' && word[i] != '\"')
        {
            word[j] = word[i];
            j++;
        }
        i++;
    }
    word[j] = '\0';
    return word;
}


int	init_and_collect_word(t_parse_context *ctx, char **word)
{
	*word = NULL;
	if (collect_word(ctx->input, ctx->i, word, ctx->shell))
		return (1);
	if (!*word || ft_strlen(*word) == 0)
	{
		free(*word);
		return (1);
	}
	return (0);
}

int	handle_non_heredoc(t_shell_data *shell, char *word, t_expanded_words *words)
{
	if (save_and_expand_word(shell, word, &(words->expanded), &(words->original)))
	{
		free(word);
		return (1);
	}
	return (0);
}

int	process_heredoc_word(t_shell_data *shell, char *word, \
t_expanded_words *words)
{
	rm_quotes(word);
	words->expanded = ft_strdup(word);
	words->original = ft_strdup(word);
	if (!words->expanded || !words->original)
	{
		if (words->expanded)
			free(words->expanded);
		if (words->original)
			free(words->original);
		shell->error_flag = 4;
		free(word);
		return (1);
	}
	return (0);
}

int	collect_and_expand_redirection_word(t_shell_data *shell, \
		t_parse_context *ctx, t_expanded_words *words)
{
	char	*word;
	int		error;

	error = init_and_collect_word(ctx, &word);
	if (error)
		return (1);
	if (ctx->redir->type != REDIR_HEREDOC)
	{
		error = handle_non_heredoc(shell, word, words);
		if (error)
			return (1);
	}
	else
	{
		error = process_heredoc_word(shell, word, words);
		if (error)
			return (1);
	}
	free(word);
	return (0);
}



void	assign_redirection(t_shell_data *shell, char *expanded_word, \
char *original_word)
{
	shell->filename_or_delimiter = expanded_word;
	free(original_word);
}

int	validate_expanded_word(t_shell_data *shell, char *expanded_word)
{
	if (is_valid_operator(expanded_word) ||
		starts_with_operator_char(expanded_word[0]))
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `",
					STDERR_FILENO);
		ft_putstr_fd(expanded_word, STDERR_FILENO);
		ft_putstr_fd("'\n", STDERR_FILENO);
		shell->exit_status = 2;
		return (1);
	}
	return (0);
}


int process_filename_or_delimiter(t_shell_data *shell, char *input,
                                  int *i, t_redirection *redir)
{
    t_expanded_words    words;
    t_parse_context     ctx;

    ctx.shell = shell;
    ctx.input = input;
    ctx.i = i;
    ctx.redir = redir;
    if (collect_and_expand_redirection_word(shell, &ctx, &words)) //[x]
        return (handle_missing_filename_error(shell, redir));
    if (!words.expanded || ft_strlen(words.expanded) == 0)
    {
        free(words.expanded);
        free(words.original);
        return (handle_missing_filename_error(shell, redir));
    }
    if (validate_expanded_word(shell, words.expanded))
    {
        free(words.expanded);
        free(words.original);
        free_redirections(redir);
        return (1);
    }
    assign_redirection(shell, words.expanded, words.original);
    return (0);
}

int	handle_unexpected_token_error(t_shell_data *shell, t_redirection *new_redir,
								char *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
	ft_putstr_fd(token, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
	free(new_redir);
	shell->exit_status = 2;
	return (1);
}

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

int handle_syntax_error_s(t_shell_data *shell, t_redirection *new_redir, \
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

void	skip_whitespace(char *input, int *i)
{
	while (input[*i] && ft_isspace(input[*i]))
		(*i)++;
}

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

int	finalize_redirection(t_shell_data *shell, t_redirection *new_redir)
{
    if (new_redir->type == REDIR_HEREDOC)
	{
		if (handle_heredoc_redirection(shell, new_redir, shell->filename_or_delimiter))
			return (1);
	}
	else
		new_redir->filename = shell->filename_or_delimiter;
	shell->filename_or_delimiter = NULL;
	return (0);
}



int	handle_pipe_op(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '|')
		return (handle_syntax_error_s(shell, new_redir, "||"));
	else if (input[*i + 1] == '>')
		return (handle_syntax_error_s(shell, new_redir, ">|"));
	else if (input[*i + 1] == '<')
		return (handle_syntax_error_s(shell, new_redir, "<|"));
	else if (input[*i + 1] == '\0' || input[*i + 1] == ' ' || \
    input[*i + 1] == '\n')
		return (handle_syntax_error_s(shell, new_redir, "|"));
	else
		return (handle_syntax_error_s(shell, new_redir, "|"));
}

int	handle_greater_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '>')
		return (handle_syntax_error_s(shell, new_redir, ">>"));
	else
		return (handle_syntax_error_s(shell, new_redir, ">"));
}

int	handle_less_operator(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i + 1] == '<')
		return (handle_syntax_error_s(shell, new_redir, "<<"));
	else
		return (handle_syntax_error_s(shell, new_redir, "<"));
}

int	check_operator_error(t_shell_data *shell, char *input, int *i, \
t_redirection *new_redir)
{
	if (input[*i] == '|')
		return (handle_pipe_op(shell, input, i, new_redir));
	else if (input[*i] == '>')
		return (handle_greater_operator(shell, input, i, new_redir));
	else if (input[*i] == '<')
		return (handle_less_operator(shell, input, i, new_redir));
	return (0);
}

int	prepare_redirection(t_command *cmd, t_redirection **new_redir)
{
    char *op = cmd->current_op;

    if (!op)
        return (1);
    *new_redir = create_new_redirection(op);
    free(op);
    cmd->current_op = NULL;
    return (!(*new_redir));
}


int handle_redirection(t_shell_data *shell, char *input, int *i, \
t_command *cmd)
{
    t_redirection *new_redir;

    shell->filename_or_delimiter = NULL;
    if (prepare_redirection(cmd, &new_redir))
        return (1);
    
    skip_whitespace(input, i);
    if (check_operator_error(shell, input, i, new_redir))
        return (1);
    if (process_filename_or_delimiter(shell, input, i, new_redir))
        return (1);
    if (finalize_redirection(shell, new_redir))
        return (1);
    add_redirection(&(cmd->redirections), new_redir);
    return (0);
}

int is_redirection_operator(char *op)
{
    return (!ft_strcmp(op, "<") || !ft_strcmp(op, ">") ||
            !ft_strcmp(op, ">>") || !ft_strcmp(op, "<<"));
}

int	add_token(char *token_value, t_token **token_list,
		int *index, int is_operator)
{
	t_token	*new_token;
	t_token	*current;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (1);
	}
	initialize_new_token(new_token, token_value, index, is_operator);
	if (!*token_list)
		*token_list = new_token;
	else
	{
		current = *token_list;
		while (current->next)
			current = current->next;
		current->next = new_token;
	}
	return (0);
}

void	initialize_new_token(t_token *new_token, char *token_value,
		int *index, int is_operator)
{
	new_token->value = token_value;
	new_token->original_value = NULL;
	new_token->index = (*index)++;
	new_token->is_operator = is_operator;
	new_token->is_command = 0;
	new_token->is_flag = 0;
	new_token->is_int = 0;
	new_token->var_not_found = 0;
	new_token->wrong_operator = 0;
	new_token->is_heredoc = 0;
	new_token->heredoc_delimiter = NULL;
	new_token->heredoc_file = NULL;
	new_token->is_end = 0;
	new_token->next = NULL;
}
#include "../minishell.h"

void handle_ctrl_d(t_shell_data *shell)
{
    cleanup(shell);
    exit(shell->exit_status);
}

int	process_and_execute_commands(t_shell_data *shell)
{
    if (preprocess_input(shell) != 0)
	{
		return 1;
	}
	// printf("\nDebug: shell->cmds_nb: %d\n\n", shell->cmds_nb);
	// printf("\nDebug: shell->pipe_nb: %d\n\n", shell->pipe_nb);
	//print_commands(shell);
	if (shell->commands)
	{
		//printf("Debug: Starting execution()---------------------------------------\n");
		execution(shell); // Currently commented out
		//printf("Debug: Finished execution()-------------------------------------\n");
	}
	else
		shell->exit_status = 2;
	free_shell_resources(shell);
	return (0);
}

int	check_and_handle_syntax_errors(t_shell_data *shell)
{
	if (check_unclosed_quotes(shell->input))
	{
		check_syntax_error(shell, "minishell: syntax error: unclosed quotes\n");
		cleanup(shell);
		return (1);
	}
	else if (check_leading_pipe(shell->input))
	{
		check_syntax_error(shell, "syntax error near unexpected token `|'\n");
		cleanup(shell);
		return (1);
	}
	else if (check_trailing_pipe(shell->input))
	{
		check_syntax_error(shell, "minishell: syntax error near unexpected token `|'\n");
		cleanup(shell);
		return (1);
	}
	return (0);
}

int check_input_length(t_shell_data *shell)
{
    if (ft_strlen(shell->input) > MAX_INPUT_LENGTH)
    {
        ft_putstr_fd("minishell: input too long\n", STDERR_FILENO);
        free(shell->input);
        return (1);
    }
    return (0);
}

void	add_to_history_if_needed(t_shell_data *shell)
{
	if (ft_strlen(shell->full_input) > 0)
		add_history(shell->full_input);
}

int handle_allocation(t_shell_data *shell)
{
    shell->full_input = ft_strdup(shell->input);
    if (!shell->full_input)
    {
        ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
        return (1);
    }
    return (0);
}

int read_input(t_shell_data *shell)
{
    shell->input = readline(PROMPT);
    if (!shell->input)
    {
        handle_ctrl_d(shell);
        return (1);
    }
    return (0);
}

int	allocate_resources(t_shell_data *shell)
{
	if (handle_allocation(shell))
	{
		return (1);
	}
	return (0);
}

int	validate_input_length(t_shell_data *shell)
{
	if (check_input_length(shell)) 
	{
		cleanup(shell);
		return (1);
	}
	return (0);
}


void	handle_empty_input(t_shell_data *shell)
{
	free(shell->input);
	shell->input = NULL;
	g_signal.signal_status = 0;
}

int	is_input_empty(t_shell_data *shell)
{
	return (shell->input == NULL || shell->input[0] == '\0'); 
}


int	handle_execution_flow(t_shell_data *shell)
{
	g_signal.signal_status = 0;
	if (validate_input_length(shell))
		return (1);
	if (allocate_resources(shell))
		return (1);
	add_to_history_if_needed(shell);
	if (check_and_handle_syntax_errors(shell))
		return (1);
	if (process_and_execute_commands(shell) != 0)
	{
		if (shell->error_flag == 2 || shell->error_flag == 3)
			return (0);
		return (1);
	}
	return (1);
}

int	handle_user_input(t_shell_data *shell)
{	
	if (is_input_empty(shell))
	{
		handle_empty_input(shell);
	}
	else
	{
		return (handle_execution_flow(shell));
	}
	return (1);
}

int	handle_read_error(void)
{
	ft_putstr_fd("Error: Failed to read input\n", STDERR_FILENO);
	return (0); 
}

int	handle_input(t_shell_data *shell)
{
	int	running;
	int	status;

	running = 1;
	while (running)
	{
		status = read_input(shell);
		if (status == -1)
			running = handle_read_error();
		else
		{
			running = handle_user_input(shell);
			setup_signal_handlers(0);
		}
	}
	rl_clear_history();
	return (0);
}
#include "../minishell.h"

int	calc_pipe_nb(t_shell_data *shell)
{
	char	*full_input;
	int		pipe_nb;

	full_input = shell->full_input;
	pipe_nb = 0;
	while (*full_input)
	{
		if (*full_input == '|')
			pipe_nb++;
		full_input++;
	}
	return (pipe_nb);
}

int	calc_cmds_nb(t_shell_data *shell)
{
	char	*input;
	int		cmd_count;
	int		i;

	input = shell->full_input;
	cmd_count = 0;
	i = 0;
	if (!input || !*input)
		return (0);
	while (input[i])
	{
		while (input[i] == ' ' || input[i] == '\t')
			i++;
		if (input[i] && input[i] != '|')
		{
			cmd_count++;
			while (input[i] && input[i] != '|')
				i++;
		}
		if (input[i] == '|')
			i++;
	}
	return (cmd_count);
}

void	init_shell(t_shell_data *shell)
{
    shell->input = NULL;
    shell->full_input = NULL;
    shell->exit_status = 0;
    shell->commands = NULL;
    shell->error_flag = 0;
    shell->signal_status = 0;
    shell->in_single_quote = 0;
    shell->in_double_quote = 0;
    shell->prev_char = '\0';
    shell->var_not_found_flag = 0;
	shell->prev_dir = getcwd(NULL, 0);
	shell->in_heredoc = 0;
	shell->expanded = 0;
	shell->double_quoted = 0;
	shell->original_word = NULL;
	if (!shell->prev_dir)
	{
		perror("getcwd");
		exit(EXIT_FAILURE);
	}
}
#include "../minishell.h"

int	check_leading_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = 0;
	while (input[i] && ft_isspace(input[i]))
		i++;
	if (input[i] == '|')
		return (1);
	return (0);
}

int	check_trailing_pipe(char *input)
{
	int	i;

	if (!input)
		return (0);
	i = ft_strlen(input) - 1;
	while (i >= 0 && ft_isspace(input[i]))
		i--;
	if (i >= 0 && input[i] == '|')
		return (1);
	return (0);
}
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

int is_valid_identifier(const char *str, int allow_equals) {
    int i = 0;

    if (!str || (!ft_isalpha(str[i]) && str[i] != '_'))
        return 0;
    while (str[i]) {
        if (!ft_isalnum(str[i]) && str[i] != '_') 
		{
            if (allow_equals && str[i] == '=')
                return 1;
            return 0;
        }
        i++;
    }
    if (!allow_equals && ft_strchr(str, '='))
        return 0;
    return 1;
}

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


void	add_to_env(t_shell_data *shell, const char *str)
{
	char	*key;
	int		key_len;
	int		index;

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
	{
		replace_env_var(shell, index, str);
	}
	else
	{
		int	size = 0;
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
	return (ft_strncmp(env_var, name, name_len) == 0 &&
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


static int	get_env_size(char **envp)
{
	int	size;

	size = 0;
	while (envp[size])
		size++;
	return (size);
}

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
			ft_putstr_fd("minishell: export: allocation error\n", STDERR_FILENO);
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
#include "internal_commands.h"

int	ft_exit_child(t_shell_data *shell, t_command *cmd)
{
    (void)cmd;
	int		exit_status;

	exit_status = shell->exit_status;
	cleanup(shell);
	rl_clear_history();
    close(shell->saved_stdin);
    close(shell->saved_stdout);
	exit(exit_status);
}
void free_pid_list(t_shell_data *shell)
{
    t_pid_node *current;
    t_pid_node *next;

    current = shell->pid_list;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    shell->pid_list = NULL;
}


int fork_and_execute(t_shell_data *shell, t_command *cmds, t_token *token)
{
    pid_t pid;

    signal(SIGPIPE, SIG_IGN);
    pid = fork();
    if (pid < 0)
    {
        write_error("Fork failed", strerror(errno));
        return (-1);
    }
    else if (pid == 0)
    {
        close_all_pipes(shell->pipes, shell->cmds_nb);
        if (execute_command(shell, cmds, token, 0) == -1)
        {
            free_pid_list(shell);
            ft_exit_child(shell, cmds);
        }
        free_pid_list(shell);
        ft_exit_child(shell, cmds);
    }
    else
        store_pids(shell, pid);
    return (0);
}


int execute_parent_command(t_shell_data *shell, t_command *cmds, \
t_token *token)
{
    if (!shell || !cmds || !token)
        return (-1);
    if(shell->cmds_nb > 1)
        return (1);
    return (execute_command(shell, cmds, token, 0));
}

int is_parent_command(const char *cmd)
{
    return (ft_strcmp(cmd, "cd") == 0 ||
            ft_strcmp(cmd, "exit") == 0 ||
            ft_strcmp(cmd, "export") == 0 ||
            ft_strcmp(cmd, "unset") == 0);
}


/*
shell->exit_status = ret
----> Set global exit status based on command return value
*/
int	execute_command(t_shell_data *shell, t_command *cmd,
				t_token *token, int ret)
{
	if (!shell || !cmd || !token)
        return (-1);
	if (ft_strcmp(token->value, "echo") == 0)
		ret = ft_echo(shell, cmd);
	else if (ft_strcmp(token->value, "cd") == 0)
		ret = ft_cd(shell, cmd);
	else if (ft_strcmp(token->value, "pwd") == 0)
		ret = ft_pwd(shell, cmd);
	else if (ft_strcmp(token->value, "export") == 0)
		ret = ft_export(shell, cmd);
	else if (ft_strcmp(token->value, "unset") == 0)
		ret = ft_unset(shell, cmd);
	else if (ft_strcmp(token->value, "env") == 0)
		ret = ft_env(shell, cmd);
	else if (ft_strcmp(token->value, "exit") == 0)
		ret = ft_exit_shell(shell, cmd);
	else
	{
		write_error(token->value, ": No such file or directory\n");
		shell->exit_status = 127;
		return (-1);
	}
	shell->exit_status = ret;
	return (ret);
}

int execute_internal_commands(t_shell_data *shell, t_command *cmds)
{
    t_token *token;

    if (!cmds || !cmds->token_list)
    {
		return (0);
	}
    token = cmds->token_list;
    while (token)
    {
        if (token->is_command)
        {
			if (is_parent_command(token->value))
            {
                if (execute_parent_command(shell, cmds, token) == -1)
                    return (-1);
            }
			else
            {
				if (fork_and_execute(shell, cmds, token) == -1)
                    return (-1);
            }
        }
        token = token->next;
    }
    return 0;
}
#include "../internal_commands.h"


int	ft_pwd(t_shell_data *shell,t_command *cmd)
{
	char	cwd[PATH_MAX];

	(void)cmd;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
		shell->exit_status = 0;
	}
	else
	{
		perror("minishell: pwd");
		shell->exit_status = 1;
	}
	return (0);
}
#include "../internal_commands.h"

void	print_exit_error(char *message, int *exit_status, int code)
{
	ft_putstr_fd(message, STDERR_FILENO);
	*exit_status = code;
}

void print_exit_error2(char *format, char *arg, int *exit_status, int code)
{
    fprintf(stderr, format, arg);
    *exit_status = code;
}

/*
** Helper function to replicate strtoll behavior.
** Converts a string to long long while checking for overflows and invalid input.
*/

int	initialize_conversion(const char **str, int *sign)
{
    while (**str == ' ' || **str == '\t' || **str == '\n')
        (*str)++;
    *sign = 1;
    if (**str == '-')
    {
        *sign = -1;
        (*str)++;
    }
    else if (**str == '+')
    {
        *sign = 1;
        (*str)++;
    }
    if (!ft_isdigit(**str))
        return (0);
    return (1);
}

int	parse_number(const char *str, long long *num, int sign)
{
    long long	digit;

    while (ft_isdigit(*str))
    {
        digit = *str - '0';
        if (sign == 1 && (*num > (LLONG_MAX - digit) / 10))
            return (0);
        if (sign == -1 && (-*num < (LLONG_MIN + digit) / 10))
            return (0);
        *num = (*num * 10) + digit;
        str++;
    }
    while (*str == ' ' || *str == '\t' || *str == '\n')
        str++;
    if (*str != '\0')
        return (0);
    return (1);
}

int	str_to_long_long(const char *str, long long *result)
{
    int			sign;
    long long	num;

    num = 0;
    *result = 0;
    if (!initialize_conversion(&str, &sign))
        return (0);
    if (!parse_number(str, &num, sign))
        return (0);
    *result = num * sign;
    return (1);
}

void	cleanup_and_exit(t_shell_data *shell, int exit_status)
{
	cleanup(shell);
	rl_clear_history();
	close(shell->saved_stdin);
	close(shell->saved_stdout);
	exit(exit_status);
}

int	process_exit_arguments(t_shell_data *shell, t_command *cmd, int *exit_status)
{
	t_token	*token;

	token = cmd->token_list->next;
	*exit_status = shell->exit_status;
	if (token)
	{
		if (is_numeric(token->value))
		{
			*exit_status = ft_atoi(token->value) % 256;
			token = token->next;
			if (token)
			{
				print_exit_error("exit: too many arguments\n",
					&shell->exit_status, 1);
				return (1);
			}
		}
		else
		{
			print_exit_error2("exit: %s: numeric argument required\n",
				token->value, exit_status, 2);
		}
	}
	return (0);
}

int	ft_exit_shell(t_shell_data *shell, t_command *cmd)
{
	int	exit_status;
	int	should_not_exit;

	should_not_exit = process_exit_arguments(shell, cmd, &exit_status);
	if (should_not_exit)
		return (1);
	cleanup_and_exit(shell, exit_status);
	return (0); 
}

int	is_numeric(const char *str)
{
	long long num;

	return str_to_long_long(str, &num);
}
#include "../internal_commands.h"

//compute the length of the initial segment of a string 
// that consists entirely of characters from a specified set.
size_t ft_strspn(const char *str1, const char *str2)
{
    size_t count = 0;
    const char *p;

    while (*str1) {
        for (p = str2; *p; ++p) {
            if (*str1 == *p)
                break;
        }
        if (!*p)
            break;
        ++str1;
        ++count;
    }
    return count;
}

int is_valid_n_flag(const char *value)
{
    if (value[0] != '-')
        return (0);
    if (value[1] != 'n')
        return (0);
    if (ft_strspn(value + 1, "n") != strlen(value + 1))
        return (0);
    return (1);
}

/*
while (token && ft_strcmp(token->value, "-n") == 0)-> Handle multiple -n options
while (token)                                      -> Print arguments
Only print the newline if we're in interactive mode
    	if (newline && shell->interactive_mode)

*/
int	ft_echo(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;
	int		newline;

	token = cmd->token_list->next;
	newline = 1;
	while (token && is_valid_n_flag(token->value))
	{
		newline = 0;
		token = token->next;
	}
	while (token)
	{
		printf("%s", token->value);
		if (token->next)
			printf(" ");
		token = token->next;
	}
    if (newline)
		printf("\n");
	if (shell->interactive_mode)
		printf("\n");
	return (0);
}
#include "../internal_commands.h"

int handle_cd_minus(t_shell_data *shell)
{
    char *current_dir;

    if (shell->prev_dir == NULL)
    {
        ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
        shell->exit_status = 1;
        return (1);
    }
    current_dir = getcwd(NULL, 0);
    if (chdir(shell->prev_dir) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        perror(shell->prev_dir);
        shell->exit_status = 1;
        return (1);
    }
    printf("%s\n", shell->prev_dir);
    if (current_dir)
    {
        free(shell->prev_dir);
        shell->prev_dir = current_dir;
    }
    shell->exit_status = 0;
    return (0);
}

/*
	expanded_path = ft_strdup(home);                // Path is just "~"
	expanded_path = ft_strjoin(home, &path[1]);     // Path starts with "~/"
	in handle_exec_error:
	ft_putstr_fd("minishell: cd: no such file or directory: ", STDERR_FILENO)
	-> Handle cases like "~user" (optional)

*/
int	handle_tilde_path(t_shell_data *shell, char *path)
{
	char	*home;
	char	*expanded_path;

	home = getenv_from_envp(shell, "HOME");
	if (!home)
		return (handle_no_home(shell));
	if (path[1] == '\0')
		expanded_path = ft_strdup(home);
	else if (path[1] == '/')
		expanded_path = ft_strjoin(home, &path[1]);
	else
	{
		write_error(path, "\n");
		shell->exit_status = 1;
		return (1);
	}
	if (!expanded_path)
		return (handle_memory_error(shell));
	if (chdir(expanded_path) != 0)
		return (handle_chdir_error(shell, expanded_path));
	free(expanded_path);
	shell->exit_status = 0;
	return (0);
}

int	change_to_home(t_shell_data *shell)
{
	char	*home;

	home = getenv_from_envp(shell, "HOME");
	if (!home)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		shell->exit_status = 1;
		return (1);
	}
	if (chdir(home) != 0)
	{
		perror("minishell: cd");
		shell->exit_status = 1;
		return (1);
	}
	shell->exit_status = 0;
	return (0);
}

/*
	token = cmd->token_list->next;         // Skip the command token
	path = token->value;                   // Argument provided
	if (chdir(path) != 0)                  // Regular path
*/


static int validate_path(const char *path)
{
    if (strstr(path, "..$"))
    {
        ft_putstr_fd((char *)path, STDERR_FILENO);
        ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
        return (1);
    }
    return (0);
}

static int update_prev_dir(t_shell_data *shell, char *current_dir)
{
    if (current_dir)
    {
        free(shell->prev_dir);
        shell->prev_dir = current_dir;
    }
    return (0);
}

int ft_cd(t_shell_data *shell, t_command *cmd)
{
    t_token *token;
    char *path;
    char *current_dir;

    token = cmd->token_list->next;
    if (!token)
        return change_to_home(shell);
    if (token->next != NULL)
        return (ft_putstr_fd("minishell: cd: too many arguments\n", STDERR_FILENO), shell->exit_status = 1, 1);
    path = token->value;
    if (validate_path(path))
        return (shell->exit_status = 1);
    if (strcmp(path, "-") == 0)
        return handle_cd_minus(shell);
    if (path[0] == '~')
        return handle_tilde_path(shell, path);
    current_dir = getcwd(NULL, 0);
    if (chdir(path) != 0)
    {
        ft_putstr_fd("minishell: cd: ", STDERR_FILENO);
        perror(path);
        return (free(current_dir), shell->exit_status = 1);
    }
    return update_prev_dir(shell, current_dir);
}
#include "../../minishell.h"

int	handle_chdir_error(t_shell_data *shell, char *expanded_path)
{
	perror("minishell: cd");
	free(expanded_path);
	shell->exit_status = 1;
	return (1);
}

int	handle_memory_error(t_shell_data *shell)
{
	ft_putstr_fd("minishell: cd: memory allocation error\n", STDERR_FILENO);
	shell->exit_status = 1;
	return (1);
}

int	handle_no_home(t_shell_data *shell)
{
	ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
	shell->exit_status = 1;
	return (1);
}
// src/ft_env.c
#include "../internal_commands.h"

/*
1. Skip the command token
2. while (token) :  Count the number of arguments provided to env
3. while (shell->envp[i]) : No arguments provided; print all env_ variables
*/

void	print_env(t_shell_data *shell)
{
	int	i;

	i = 0;
	while (shell->envp[i])
	{
		ft_putstr_fd(shell->envp[i], STDOUT_FILENO);
		ft_putchar_fd('\n', STDOUT_FILENO);
		i++;
	}
}

int	ft_env(t_shell_data *shell, t_command *cmd)
{
	int	arg_count;
	t_token	*token;

	token = cmd->token_list->next;
	arg_count = 0;
	while (token)
	{
		arg_count++;
		token = token->next;
	}
	if (arg_count > 0)
	{
		shell->exit_status = 1;
		return (1);
	}
	print_env(shell);
	shell->exit_status = 0;
	return (0);
}
#include "../internal_commands.h"

int	ft_export(t_shell_data *shell, t_command *cmd)
{
	t_token	*token;

	token = cmd->token_list->next;
	if (!token)
	{
		print_sorted_env(shell);
		shell->exit_status = 0;
		return (0);
	}
	while (token)
	{
		if (is_valid_identifier(token->value, 1))
			add_to_env(shell, token->value);
		else
		{
			ft_putstr_fd("minishell: export: `", STDERR_FILENO);
			ft_putstr_fd(token->value, STDERR_FILENO);
			ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
			shell->exit_status = 1;
			return (1);
		}
		token = token->next;
	}
	return (0);
}
#include "../internal_commands.h"

int ft_unset(t_shell_data *shell, t_command *cmd)
{
    t_token *token;

    token = cmd->token_list->next;
    while (token)
    {
        if (is_valid_identifier(token->value, 0))
            remove_from_env(shell, token->value);
        else
            return (0);
        token = token->next;
    }
    return (0);
}
#include "../minishell.h"

int	is_internal_command(char *token)
{
	if (!ft_strcmp(token, "echo") || !ft_strcmp(token, "cd")
		|| !ft_strcmp(token, "pwd") || !ft_strcmp(token, "export")
		|| !ft_strcmp(token, "unset") || !ft_strcmp(token, "env")
		|| !ft_strcmp(token, "exit"))
		return (1);
	return (0);
}
#include "../minishell.h"

void free_argv(char **argv, int count)
{
	int i;

	i = 0;
	if (!argv)
        return ;
    while (i < count)
    {
		if (argv[i])
        {
			free(argv[i]);
			argv[i] = NULL;
		}
		i++;
	}
    free(argv);
}

/*
Flow of Execution
Parent Process:

Forks a child.
Tracks the child's PID for cleanup later.
Child Process:

Closes unused pipes.
Executes the command using execve().
On failure, exits with the appropriate error code.
Post-Execution:

After all commands in a pipeline are forked, the parent waits
for all child processes using waitpid().
*/
int	token_list_length(t_token *token)
{
	int	count;

	count = 0;
	while (token)
	{
		count++;
		token = token->next;
	}
	return (count);
}

char	*resolve_command_path(t_shell_data *shell, t_command *cmds,
				char **arr_token)
{
	char	*cmd_path;
	struct stat path_stat;

	cmd_path = get_command_path(shell, cmds->token_list);
	if (!cmd_path)
	{
		write_error(arr_token[0], "command not found");
		shell->exit_status = 127;
		free_argv(arr_token, token_list_length(cmds->token_list));
		return (NULL);
	}
	if (cmd_path[0] == '.' && (cmd_path[1] == '\0' || (cmd_path[1] == '.' && cmd_path[2] == '\0')))
    {
        write_error(cmd_path, "command not found");
        shell->exit_status = 127;
        free(cmd_path);
        free_argv(arr_token, token_list_length(cmds->token_list));
        return (NULL);
    }
	if (stat(cmd_path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
    {
        write_error(cmd_path, "Is a directory");
        shell->exit_status = 126;
        free(cmd_path);
        free_argv(arr_token, token_list_length(cmds->token_list));
        return (NULL);
    }
	return (cmd_path);
}


int	convert_tokens_to_argv(t_token *token_list, char **argv)
{
    int		i;
    t_token	*temp;

    i = 0;
    temp = token_list;
    while (temp)
    {
        argv[i] = ft_strdup(temp->value);
        if (!argv[i])
        {
            free_argv(argv, i);
            return (-1);
        }
        temp = temp->next;
        i++;
    }
    argv[i] = NULL;
    return (0);
}

/*
** If variable exists, replace it.
** If it doesn't, add it at the end.
*/

void update_env_variable(t_shell_data *shell, const char *key, const char *value)
{
    int   key_len;
    int   index;
    char  *new_entry;

    // Prepare key and new_entry -> "key=value"
    key_len = ft_strlen(key);
    new_entry = ft_strjoin3(key, "=", value); 
    // You can create ft_strjoin3 or do manual memory for "key=value"

    if (!new_entry)
        return ; // Or handle error

    // Check if key exists in envp
    index = find_env_index(shell->envp, key, key_len); // you already have something similar
    if (index != -1)
    {
        // Replace existing
        free(shell->envp[index]);
        shell->envp[index] = new_entry;
    }
    else
    {
        // Add new at the end
        int size = 0;
        while (shell->envp[size])
            size++;

        // Realloc / create bigger array
        char **new_envp = allocate_new_envp(size, shell);
        // ^ same idea you use in add_new_env_var
        if (!new_envp)
        {
            free(new_entry);
            return ;
        }
        copy_existing_envp(new_envp, shell->envp, size);
        finalize_new_envp(new_envp, new_entry, size);
        replace_envp(shell, new_envp);
    }
}


/*
We need to close all the pipes in the child process because: we already 
changed the stdout and stdin inside set_redirs and set_pipes()

 If execve call is successful, the following lines are never executed
 chon : the child process will kill itself when finished
*/
void	exec_external_child(t_shell_data *shell, char *cmd_path, char **argv)
{
	int	error_code;
	int	exit_status;

	//pouya
	char *underscore_var;

	close_all_pipes(shell->pipes, shell->cmds_nb);
	// pouya
	underscore_var = ft_strjoin3("_", "=", cmd_path);
	if (!underscore_var)
    {
        write_error("minishell", "Memory allocation error");
        exit(shell->exit_status = 1);
    }
	add_to_env(shell, underscore_var);
	free(underscore_var);
	exit_status = execve(cmd_path, argv, shell->envp);
	if (exit_status == -1)
	{
		error_code = get_exec_error_code(errno);
		free(cmd_path);
		write_error(argv[0], strerror(errno));
		free(argv);
		argv = NULL;
		shell->exit_status = error_code;
	}
}

void	execute_external_commands(t_shell_data *shell, t_command *cmds)
{
	char	*cmd_path;
	char	**arr_token;
	pid_t	pid;
	int token_count;

	token_count = token_list_length(cmds->token_list);
    arr_token = malloc(sizeof(char *) * (token_count + 1));
    if (!arr_token)
    {
        write_error("Memory allocation failed", strerror(errno));
        return;
    }
    if (convert_tokens_to_argv(cmds->token_list, arr_token) == -1)
    {
        free(arr_token);
        return ;
    }
    if (!arr_token[0])
    {
        free(arr_token);
        return ;
    }
	cmd_path = resolve_command_path(shell, cmds, arr_token);
	if (!cmd_path)
    {
		//free(cmd_path);
        // free(arr_token);
        return;
    }

	char *underscore_var = ft_strjoin("_=", cmd_path);
    if (underscore_var)
    {
        add_to_env(shell, underscore_var);
        free(underscore_var);
    }
    else
    {
        // Handle memory allocation failure if necessary
        write_error("minishell: export: allocation error", "");
    }

	pid = fork();
	if (pid < 0)
	{
		write_error("Fork failed", strerror(errno));
		free(cmd_path);
		free_argv(arr_token, token_count);
		return ;
	}
	else if (pid == 0)
	{
		exec_external_child(shell, cmd_path, arr_token);
		exit(shell->exit_status);
	}
	else
		store_pids(shell, pid);
	free(cmd_path);
	free_argv(arr_token, token_count);
}

void	forking(t_shell_data *shell, t_command *cmds)
{
	setup_signal_handlers(2);
	if (cmds->token_list && cmds->token_list->is_int)
	{
		execute_internal_commands(shell, cmds);
	}
	else
	{
		execute_external_commands(shell, cmds);
	}
}
#include "../minishell.h"

void	store_pids(t_shell_data *shell, pid_t pid)
{
	t_pid_node	*new_node;
	t_pid_node	*current;

	new_node = (t_pid_node *)malloc(sizeof(t_pid_node));
	if (!new_node)
	{
		perror("Failed to allocate memory for PID node");
		return ;
	}
	new_node->pid = pid;
	new_node->next = NULL;
	if (shell->pid_list == NULL)
	{
		shell->pid_list = new_node;
	}
	else
	{
		current = shell->pid_list;
		while (current->next != NULL)
			current = current->next;
		current->next = new_node;
	}
}
#include "../minishell.h"

// Join two strings and free the first one after joining
char *join_path(const char *prefix, const char *suffix)
{
    char *result;
    char *temp;
    
    temp = ft_strjoin(prefix, "/");
    if (!temp)
        return (NULL);
    result = ft_strjoin(temp, suffix);
    free(temp);
    return (result);
}

// Check if a path exists and is not a directory
int is_valid_file(const char *path)
{
    struct stat st;
    return (stat(path, &st) == 0 && !S_ISDIR(st.st_mode));
}

char *check_and_return_path(const char *cmd, char **all_paths)
{
    char *path_to_search;
    char *final_path;
    struct stat st;
    int i;

    i = 0;
    final_path = NULL;
    while (all_paths[i])
    {
        path_to_search = join_path(all_paths[i], cmd);
        if (!path_to_search)
            return (NULL);
        if (access(path_to_search, F_OK) == 0 && is_valid_file(path_to_search))
        {
            final_path = ft_strdup(path_to_search);
            free(path_to_search);
            break;
        }
        if (stat(path_to_search, &st) == 0 && S_ISDIR(st.st_mode)) // Check if it's a directory
        {
            free(path_to_search);
            break;
        }
        free(path_to_search);
        i++;
    }
    return (final_path);
}


char	**get_paths_from_env(t_shell_data *shell, char **env)
{
	int	i;
	char	**all_paths;

	i = 0;
    all_paths = NULL;
	while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i])
		all_paths = ft_split(env[i] + 5, ':');
    else if (shell->prev_dir)
        all_paths = ft_split(shell->prev_dir, ':');
    return (all_paths);
}

char	*find_path_in_env(t_shell_data *shell, char *cmd)
{
	char	**all_paths;
	char	*path;

    path = NULL;
	all_paths = get_paths_from_env(shell, shell->envp);
	if (!all_paths)
        return (NULL);
    path = check_and_return_path(cmd, all_paths);
    if (!path)
    {
        free_paths(all_paths);
        return (NULL);
    }
	free_paths(all_paths);
	return (path);
}

char	*get_command_path(t_shell_data *shell, t_token *token)
{
	// If the token starts with a '.', check if it's a valid path
	if (token->value[0] == '.')
	{
		// Check if it is "." or ".." and return NULL (not a valid command)
		if ((token->value[1] == '\0') || (token->value[1] == '.' && token->value[2] == '\0'))
			return (NULL);

		// Check if there is a valid path after the '.' (e.g., "./file")
		if (token->value[1] == '/')
			return (ft_strdup(token->value));

		// Any other case (like ".echo.") is invalid
		return (NULL);
	}

	// If the token starts with '/', directly strdup
	if (token->value[0] == '/')
		return (ft_strdup(token->value));

	// Otherwise, find it in the environment PATH
	return (find_path_in_env(shell, token->value));
}

#include "../minishell.h"

// void	handle_exec_error(t_shell_data *shell, char *cmd, char *message,
// 			int exit_code)
// {
// 	ft_putstr_fd(cmd, STDERR_FILENO);
// 	ft_putstr_fd(": ", STDERR_FILENO);
// 	ft_putendl_fd(message, STDERR_FILENO);
// }

void	write_error(char *exec_name, char *err_message)
{
	ft_putstr_fd(exec_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(err_message, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

int	get_exec_error_code(int err)
{
	if (err == ENOENT)
		return (127);
	if (err == EACCES)
		return (126);
	return (1);
}

void free_paths(char **paths)
{
    int i;

    if (!paths)
        return ;
    i = 0;
    while (paths[i])
    {
        free(paths[i]);
        i++;
    }
    free(paths);
}

#include "../minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strdup_free(char *s1)
{
	char	*dup;

	dup = ft_strdup(s1);
	free(s1);
	return (dup);
}

char	*ft_strjoin_safe(const char *s1, const char *s2)
{
	char	*res;
	size_t	s1_len;
	size_t	s2_len;

	s1_len = 0;
	s2_len = 0;
	if (s1)
		s1_len = ft_strlen(s1);
	if (s2)
		s2_len = ft_strlen(s2);
	res = malloc(sizeof(char) * (s1_len + s2_len + 1));
	if (!res)
	{
		write(STDERR_FILENO, "minishell: malloc failed\n", 25);
		return (NULL);
	}
	if (s1)
		ft_memcpy(res, s1, s1_len);
	if (s2)
		ft_memcpy(res + s1_len, s2, s2_len);
	res[s1_len + s2_len] = '\0';
	return (res);
}

#include "../minishell.h"

void	skip_spaces(t_shell_data *shell, int *i)
{
	while (shell->input[*i] && ft_isspace(shell->input[*i]))
		(*i)++;
}

char	*ft_strjoin_and_free_both(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin_safe(s1, s2);
	if (!joined)
	{
		free(s1);
		free(s2);
		return (NULL);
	}
	free(s1);
	free(s2);
	return (joined);
}

char	*ft_strjoin_free_both(char *s1, char *s2)
{
	return (ft_strjoin_and_free_both(s1, s2));
}
