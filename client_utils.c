/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 18:21:16 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/30 11:16:54 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

char		**buffers(char **buff, char **msg)
{
	char	**parts;

	if (!ft_strlen(*buff))
	{
		ft_bzero(*msg, BUFF_SIZE);
		read(0, *msg, BUFF_SIZE);
	}
	else
		ft_bzero(*buff, 10);
	parts = ft_strsplit(msg, ' ');
}

void		in_server_connection(struct sockaddr_in addr,
		struct hostent *host, char *buff, int *sockfd)
{
	char			*msg;
	int				ret;
	char			**parts;

	ret = -1;
	msg = (char *)malloc(sizeof(char *) * BUFF_SIZE + 1);
	ft_strcpy(msg, buff);
	while (ret == -1)
	{
		parts = buffers(&buff, &msg);
		if (!ft_strncmp(msg, "/connect", 8) && parts[2])
		{
			close(*sockfd);
			*sockfd = socket(AF_INET, SOCK_STREAM, 0);
			if (ft_atoi(parts[2]) > 0 && *sockfd > 0)
			{
				addr.sin_port = htons(ft_atoi(parts[2]));
				host = gethostbyname(parts[1]);
				ft_memcpy(&addr.sin_addr.s_addr, host->h_addr,
						host->h_length);
				ret = connect(*sockfd, (void*)&(addr), sizeof(addr));
			}
		}
		(ret == -1) ? ft_putendl("Error") : ft_putendl("Connected");
	}
}

void		set_fds_conn(fd_set *master, int fd)
{
	FD_ZERO(master);
	FD_SET(fd, master);
	FD_SET(1, master);
}
