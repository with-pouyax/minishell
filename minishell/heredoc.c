#include "minishell.h"

int process_heredoc_delimiter(char *input, t_token *heredoc_token)
{
    char *delimiter;
    int start;

    while (input[g_data.i] && ft_isspace(input[g_data.i]))
        g_data.i++;

    if (!input[g_data.i])
    {
        ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
        return (1);
    }

    start = g_data.i;
    if (input[g_data.i] == '\'' || input[g_data.i] == '\"')
    {
        char quote = input[g_data.i];
        g_data.i++;
        while (input[g_data.i] && input[g_data.i] != quote)
            g_data.i++;
        if (input[g_data.i] == quote)
            g_data.i++;
        else
        {
            ft_putstr_fd("minishell: syntax error: unclosed quote in heredoc delimiter\n", STDERR_FILENO);
            return (1);
        }
    }
    else
    {
        while (input[g_data.i] && !ft_isspace(input[g_data.i]) && !is_operator_char(input[g_data.i]))
            g_data.i++;
    }

    delimiter = ft_substr(input, start, g_data.i - start);
    if (!delimiter)
        return (1);

    heredoc_token->heredoc_delimiter = delimiter;

    int ret = read_heredoc_content(heredoc_token);
    if (ret)
        return (1);

    return (0);
}



int read_heredoc_content(t_token *heredoc_token)
{
    char *line;
    int fd;
    char *tmp_filename;
    int delimiter_quoted;
    char *delimiter = heredoc_token->heredoc_delimiter;

    // Check if the delimiter is quoted
    delimiter_quoted = (delimiter[0] == '\'' || delimiter[0] == '\"');
    if (delimiter_quoted)
    {
        char *unquoted_delimiter = ft_substr(delimiter, 1, ft_strlen(delimiter) - 2);
        if (!unquoted_delimiter)
            return (1);
        free(heredoc_token->heredoc_delimiter);
        heredoc_token->heredoc_delimiter = unquoted_delimiter;
        delimiter = unquoted_delimiter;
    }

    tmp_filename = generate_temp_filename();
    if (!tmp_filename)
        return (1);

    fd = open(tmp_filename, O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
    {
        perror("minishell: heredoc");
        free(tmp_filename);
        return (1);
    }

    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
            break;

        g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, "\nheredoc> ");
        if (!g_data.full_input)
        {
            ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
            free(line);
            return (1);
        }

        g_data.full_input = ft_strjoin_and_free_first(g_data.full_input, line);
        if (!g_data.full_input)
        {
            ft_putstr_fd("Error: failed to allocate memory\n", STDERR_FILENO);
            free(line);
            return (1);
        }

        if (ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }

        if (!delimiter_quoted)
        {
            g_data.var_not_found_flag = 0;
            g_data.expansion_input = line;
            char *temp = expand_variables_in_token();
            free(line);
            if (!temp)
            {
                close(fd);
                unlink(tmp_filename);
                free(tmp_filename);
                return (1);
            }
            line = temp;
        }

        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }

    close(fd);
    heredoc_token->heredoc_file = tmp_filename;

    return (0);
}






char *generate_temp_filename(void)
{
    static int counter = 0;
    char *filename;
    char *counter_str;

    counter_str = ft_itoa(counter++);
    if (!counter_str)
        return (NULL);
    filename = ft_strjoin("/tmp/minishell_heredoc_", counter_str);
    free(counter_str);
    return (filename);
}
