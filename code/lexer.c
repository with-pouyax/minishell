#include "minishell.h"

t_token *create_token(char *value, int type)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->value = value;
    token->type = type;
    token->next = NULL;
    return (token);
}

void    add_token(t_token **head, t_token *new_token)
{
    t_token *current;

    if (!*head)
    {
        *head = new_token;
        return ;
    }
    current = *head;
    while (current->next)
        current = current->next;
    current->next = new_token;
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int tokenize_input(char *input, t_token **tokens)
{
    int     i;
    char    *value;
    t_token *token;

    i = 0;
    while (input[i])
    {
        if (is_whitespace(input[i]))
            i++;
        else if (input[i] == '\'' || input[i] == '\"')
        {
            value = get_quoted_word(input, &i);
            if (!value)
                return (1);
            token = create_token(value, WORD);
            add_token(tokens, token);
        }
        else if (is_special_char(input[i]))
        {
            if (input[i] == '>' && input[i + 1] == '>')
            {
                token = create_token(ft_strdup(">>"), APPEND_REDIRECT);
                i += 2;
            }
            else if (input[i] == '<' && input[i + 1] == '<')
            {
                token = create_token(ft_strdup("<<"), HEREDOC);
                i += 2;
            }
            else if (input[i] == '>')
            {
                token = create_token(ft_strdup(">"), OUTPUT_REDIRECT);
                i++;
            }
            else if (input[i] == '<')
            {
                token = create_token(ft_strdup("<"), INPUT_REDIRECT);
                i++;
            }
            else if (input[i] == '|')
            {
                token = create_token(ft_strdup("|"), PIPE);
                i++;
            }
            add_token(tokens, token);
        }
        else
        {
            value = get_word(input, &i);
            token = create_token(value, WORD);
            add_token(tokens, token);
        }
    }
    return (0);
}
