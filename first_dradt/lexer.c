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

#include "minishell.h"

t_token *get_special_token(char *input, int *i)
{
    t_token *token;

    if (input[*i] == '>' && input[*i + 1] == '>')
    {
        token = create_token(ft_strdup(">>"), APPEND_REDIRECT);
        (*i) += 2;
    }
    else if (input[*i] == '<' && input[*i + 1] == '<')
    {
        token = create_token(ft_strdup("<<"), HEREDOC);
        (*i) += 2;
    }
    else if (input[*i] == '>')
    {
        token = create_token(ft_strdup(">"), OUTPUT_REDIRECT);
        (*i)++;
    }
    else if (input[*i] == '<')
    {
        token = create_token(ft_strdup("<"), INPUT_REDIRECT);
        (*i)++;
    }
    else if (input[*i] == '|')
    {
        token = create_token(ft_strdup("|"), PIPE);
        (*i)++;
    }
    else
        return (NULL);
    return (token);
}


int tokenize_input(char *input, t_token **tokens)
{
    int     i;
    char    *value;
    t_token *token;
    char    quote;

    i = 0;
    quote = '\0';
    while (input[i])
    {
        if (quote == '\0' && is_whitespace(input[i]))
            i++;
        else if (input[i] == '\'' || input[i] == '\"')
        {
            quote = input[i];
            value = get_quoted_word(input, &i, quote);
            if (!value)
                return (1);
            token = create_token(value, WORD);
            add_token(tokens, token);
            quote = '\0';
        }
        else if (quote == '\0' && is_special_char(input[i]))
        {
            token = get_special_token(input, &i);
            if (!token)
                return (1);
            add_token(tokens, token);
        }
        else
        {
            value = get_word(input, &i);
            if (!value)
                return (1);
            token = create_token(value, WORD);
            add_token(tokens, token);
        }
    }
    return (0);
}

