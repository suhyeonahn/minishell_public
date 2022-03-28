#include "../includes/minishell.h"

static int	is_sep(char test, char sep)
{
	if (test == sep)
		return (1);
	return (0);
}

static int	count_words(const char *str, char c)
{
	int	cnt_words;
	int	i;

	cnt_words = 0;
	i = 0;
	while (str[i])
	{
		while (str[i] && is_sep(str[i], c))
			i++;
		if (str[i])
		{
			while (str[i] && !is_sep(str[i], c))
				i++;
			cnt_words++;
		}
	}
	return (cnt_words);
}

static char	*ft_strndup(char *src, int len)
{
	char	*cpy;
	int		i;

	i = 0;
	if (!src)
		return (NULL);
	cpy = (char *)malloc(sizeof(char) * (len + 1));
	if (!cpy)
		return (NULL);
	while (src[i] && i < len)
	{
		cpy[i] = src[i];
		i++;
	}
	cpy[i] = 0;
	return (cpy);
}

char	**ft_split(char const *s, char c)
{
	char	**tab_strings;
	int		size;
	int		len;
	int		i;

	size = count_words(s, c);
	tab_strings = (char **)malloc(sizeof(char *) * (size + 1));
	if (!tab_strings)
		return (NULL);
	i = 0;
	tab_strings[size] = 0;
	while (i < size)
	{
		len = 0;
		while (*s && is_sep(*s, c))
			s++;
		while (s[len] && !is_sep(s[len], c))
			len++;
		tab_strings[i] = ft_strndup((char *)s, len);
		if (!tab_strings[i])
			return (NULL);
		s += len;
		i++;
	}
	return (tab_strings);
}
