/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glopes-a <glopes-a@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/07 16:00:46 by glopes-a          #+#    #+#             */
/*   Updated: 2025/11/09 17:45:37 by glopes-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*handle_read_error(char *line, char *buf)
{
	free(line);
	free(buf);
	return (NULL);
}

static char	*read_join(char *line, int fd, char *buf)
{
	char	*temp;
	ssize_t	bytes_read;

	bytes_read = 1;
	while (bytes_read > 0 && (!line || !ft_strchr(line, '\n')))
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
			return (handle_read_error(line, buf));
		if (bytes_read == 0)
			break ;
		buf[bytes_read] = '\0';
		temp = ft_strjoin(line, buf);
		if (!temp)
			return (handle_read_error(line, buf));
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
	if (*fullbuf == '\0')
	{
		free(fullbuf);
		return (NULL);
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
		temp = ft_substr(*rest, pos + 1, ft_strlen(*rest) - (pos + 1));
		free(*rest);
		*rest = temp;
		return (result);
	}
	*line = ft_strdup(*rest);
	free(*rest);
	*rest = NULL;
	return (NULL);
}

char	*get_next_line(int fd)
{
	static char	*rest[FD_MAX];
	char		*result;
	char		*buf;
	char		*fullbuf;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FD_MAX)
		return (NULL);
	fullbuf = NULL;
	result = check_rest(&rest[fd], &fullbuf);
	if (result)
		return (result);
	buf = (char *)malloc(BUFFER_SIZE + 1);
	if (!buf)
	{
		free(fullbuf);
		return (NULL);
	}
	fullbuf = read_join(fullbuf, fd, buf);
	return (extract_line(fullbuf, &rest[fd]));
}
/*
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	int	fd[2];
	int				i = 0;
	char			*line;
	fd[0] = open("teste.txt", O_RDONLY);
	fd[1] = open("teste2.txt", O_RDONLY);
    while(i < 2)
    {
		if(fd[i] < 0)
		 {
        	printf("Erro ao ler fd: %d", i);
        	return (1);
		}
		i++;
    }
	i = 0;
	while(fd[i])
	{
    	while ((line = get_next_line(fd[i])) != NULL)
    	{
    	    printf("%s", line);
    	    free(line);
    	}
		i++;
	}
    close(fd[0]);
	close(fd[1]);
    return (0);
}
*/
