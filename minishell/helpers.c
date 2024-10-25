/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pouyax <pouyax@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by yourname          #+#    #+#             */
/*   Updated: 2024/10/21 15:50:47 by pouyax           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Compare two strings
int ft_strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

// Check for unclosed quotes
int check_unclosed_quotes(char *input)
{
    int in_single_quote;
    int in_double_quote;
    int i;

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

// Function to duplicate a string and free the original
char *ft_strdup_free(char *s1)
{
    char *dup;

    dup = ft_strdup(s1);
    free(s1);
    return (dup);
}

// Function to join two strings and free both
char *ft_strjoin_free(char *s1, char *s2)
{
    char *joined;

    joined = ft_strjoin(s1, s2);
    free(s1);
    free(s2);
    return (joined);
}
