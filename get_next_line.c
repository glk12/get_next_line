/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glopes-a <glopes-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/03 14:16:30 by glopes-a          #+#    #+#             */
/*   Updated: 2025/11/07 16:02:49 by glopes-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char	*read_join(char *line, int fd, char *buf)
{
	char	*temp;
	ssize_t	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0 && !ft_strchr(line, '\n'))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(line);
			free(buf);
			return (NULL);
		}
		if (bytes_read == 0)
			break ;
		buf[bytes_read] = '\0';
		temp = ft_strjoin(line, buf);
		free(line);
		line = temp;
	}
	free(buf);
	return (line);
}

static char	*extract_line(char *fullbuf, char **rest)
{
	char	*newline;
	char	*line;
	size_t	pos;

	if (!fullbuf)
		return (NULL);
	newline = ft_strchr(fullbuf, '\n');
	if (newline)
	{
		pos = newline - fullbuf;
		*rest = ft_substr(fullbuf, pos + 1, ft_strlen(fullbuf) - (pos + 1));
		line = ft_substr(fullbuf, 0, pos + 1);
		free(fullbuf);
		return (line);
	}
	return (fullbuf);
}

static char	*check_rest(char **rest, char **line)
{
	char	*temp;
	char	*result;
	size_t	pos;

	if (!(*rest))
		return (NULL);
	temp = ft_strchr(*rest, '\n');
	if (temp)
	{
		pos = temp - (*rest);
		result = ft_substr(*rest, 0, pos + 1);
		temp = ft_substr(*rest, pos + 1, ft_strlen(&(*rest)[pos + 1]));
		free(*rest);
		*rest = temp;
		return (result);
	}
	*line = ft_strdup(*rest);
	free(*rest);
	*rest = NULL;
	return (NULL);
}
;
char	*get_next_line(int fd)
{
	char		*line;
	static char	*rest;
	char		*result;
	char		*buf;
	char		*fullbuf;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	result = check_rest(&rest, &line);
	if (result)
		return (result);
	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	fullbuf = read_join(line, fd, buf);
	line = extract_line(fullbuf, &rest);
	return (line);
}
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int fd = open("teste.txt", O_RDONLY);
    if (fd < 0)
    {
        perror("Erro ao abrir arquivo");
        return (1);
    }
    char *linha;
    while ((linha = get_next_line(fd)) != NULL)
    {
        printf("%s", linha);
        free(linha);
    }
    close(fd);
    return (0);
}
