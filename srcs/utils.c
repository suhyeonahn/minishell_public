/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suahn <suahn@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/04 21:23:54 by suahn             #+#    #+#             */
/*   Updated: 2022/03/10 11:45:09 by suahn            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
		i++;
	return (i);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	i;

	i = 0;
	if (!src)
		return (0);
	while (src[i])
		i++;
	if (dstsize <= 0)
		return (i);
	if (dstsize > i)
		dstsize = i + 1;
	while (*src && dstsize-- - 1)
		*dst++ = *src++;
	*dst = '\0';
	return (i);
}

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	char			*s2;
	unsigned int	i;

	i = 0;
	if (!s)
		return (NULL);
	s2 = malloc(sizeof(char) * (len + 1));
	if (!s2)
		return (NULL);
	if (start > ft_strlen(s))
	{
		s2[0] = '\0';
		return (s2);
	}
	while (i < start)
	{
		i++;
		s++;
	}
	ft_strlcpy(s2, s, (len + 1));
	return (s2);
}

// char	*ft_substr(char *src, int start, char end_c, t_props *p)
// {
// 	int 	i;
// 	char	*s;
// 	int		j;
// 	int		quote;
// 
// 	quote = 0;
// 	i = 0;
// 	j = 0;
// 	if (end_c == 34 || end_c == 39)
// 	{
// 		quote = 2;
// 		i += 1;
// 		while(src[start + i] && src[start + i] != end_c)
// 			i++;
// 	}
// 	else
// 	{
// 		while(src[start + i] && src[start + i] != 34 && src[start + i] != 39)
// 			i++;
// 		if (src[start + i] == 34 || src[start + i] == 39)
// 			i--;
// 	}
// 	i++;
// 	s = malloc(sizeof(char) * i + 1 - quote);
// 	p->size = i;
// 	while(i--)
// 	{
// 		if (i == p->size - 1 && quote == 2)
// 			start++;
// 		if (i == 1 && quote == 2)
// 			break ;
// 		s[j++] = src[start++];
// 	}
// 	s[j] = '\0';
// 	return (s);
// }

int	ft_strcmp(char *str1, char *str2)
{
	int	i;

	i = 0;
	if (ft_strlen(str1) != ft_strlen(str2))
		return (1);
	while (str1[i])
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	return (0);
}

int	ft_strncmp(char *str1, char *str2, int n)
{
	int	i;

	i = 0;
	if (!str1 || !str2 || n <= 0)
		return (1);
	while (str1[i] && i < n)
	{
		if (str1[i] != str2[i])
			return (str1[i] - str2[i]);
		i++;
	}
	if (str2[i] && str2[i] != '=')
		return (1);
	return (0);
}

char	*ft_strcpy(char *str)
{
	char	*new_str;
	int		i;

	if (!str)
		return (NULL);
	new_str = malloc(ft_strlen(str) + 1);
	i = 0;
	while (str[i])
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

int	init_quote(char *str, int start)
{
	int	quote;

	if (str[start] == 34)
		quote = 2;
	else if (str[start] == 39)
		quote = 1;
	else
		quote = 0;
	return (quote);
}

char	end_c(int quote)
{
	char	c;

	if (quote == 2)
		c = 34;
	else if (quote == 1)
		c = 39;
	else
		c = 0;
	return (c);
}

void	free_node(t_node *node)
{
	if (node->next)
		free_node(node->next);
	free(node->s);
	free(node);
}

void	free_list(t_list *list)
{
	if (list->start)
		free_node(list->start);
	//free(list->start->s);
	free(list);
}

void	put_strs_in_list(t_list *list, char *str)
{
	(void)list;
	(void)str;
	// 	int start;
	// 	t_props p;
	// 	int	quote;
	// 
	// 	//p = malloc(sizeof(t_props));
	// 	start = 0;
	// 	quote = init_quote(str, start);
	// 	p.size = 0;
	// 	init_l(list, ft_substr(str, start, end_c(quote), &p), quote);
	// 	start = p.size;
	// 	while (str[start] != '\0')
	// 	{
	// 		quote = init_quote(str, start);
	// 		new_node(list->start,
	// 		ft_substr(str, start, end_c(quote), &p), quote);
	// 		start += p.size;
	// 	}
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*s3;
	size_t	dstsize;
	int		i;

	i = 0;
	if (!s1 || !s2)
		return (NULL);
	dstsize = ft_strlen(s2) + ft_strlen(s1);
	s3 = malloc(sizeof(char) * dstsize + 1);
	if (!s3)
		return (NULL);
	while (*s1)
	{
		s3[i++] = *s1++;
	}
	while (*s2)
	{
		s3[i++] = *s2++;
	}
	s3[i] = '\0';
	return ((char *)s3);
}

int	int_len(long n)
{
	int	len;

	len = 0;
	if (n < 0)
	{
		n *= -1;
		len++;
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		i;

	i = int_len((long)n);
	str = (char *)malloc(sizeof(char) * (i + 1));
	if (!str)
		return (NULL);
	str[i--] = '\0';
	if (n == 0)
	{
		str[0] = '0';
		return (str);
	}
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	while (n > 0)
	{
		str[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	return (str);
}

int	is_digit(const char *str)
{
	int					i;
	int					sign;
	unsigned long long	ret;

	i = 0;
	sign = 1;
	ret = 0;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-' && str[i + 1] && str[i + 1] == '-' && !str[i + 2])
			return (0);
		if ((str[i + 1] && (str[i + 1] == '-' || str[i + 1] == '+'))
			|| !str[i + 1])
			return (1);
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	if (ft_strlen(&str[i]) > 19)
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		ret = ret * 10 + str[i] - '0';
		i++;
	}
	if ((sign == -1 && ret > 9223372036854775808uLL)
		|| (sign == 1 && ret > LLONG_MAX))
		return (1);
	return (0);
}

long long	ft_atoi_for_exit(const char *str)
{
	int	i;
	int	sign;
	long long	ret;

	i = 0;
	sign = 1;
	ret = 0;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i] == '-' && str[i + 1] && str[i + 1] == '-' && !str[i + 2])
			return (0);
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		ret = ret * 10 + str[i] - '0';
		i++;
	}
	return (sign * ret);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] && (str[i] == '\n' || str[i] == '\v' || str[i] == '\t'
			|| str[i] == '\f' || str[i] == ' ' || str[i] == '\r'))
		i++;
	if (str[i] && (str[i] == '-' || str[i] == '+'))
	{
		if (str[i + 1] == '-' || str[i + 1] == '+')
			return (0);
		if (str[i] == '-')
			sign = sign * -1;
		i++;
	}
	while (str[i] && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	return (sign * result);
}
