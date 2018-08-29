/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 18:21:16 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 18:22:09 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

void		in_server_connection(struct sockaddr_in addr, struct hostent
				     *host, char *buff, int *sockfd)
{
	char			msg[BUFF_SIZE];
	char			**parts;
	int			ret;
	
	ft_strcpy(msg, buff);
	ret = -1;
	while (ret == -1)
	{
		if (!ft_strlen(buff))
		{
			ft_bzero(msg, sizeof(msg));
			read(0, msg, sizeof(msg));
		}
		else
			ft_bzero(buff, 10);
		parts = ft_strsplit(msg, ' ');
		if (!ft_strncmp(msg, "/connect", 8) && parts[2])
		{
			addr.sin_port = htons(ft_atoi(parts[2]));
			host = gethostbyname(parts[1]);
			ft_memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);
			ret = connect(*sockfd, (void*)&(addr), sizeof(addr));
		}
		(ret == -1) ? ft_putendl("Unable to connect") :
			ft_putendl("Connected");
	}
}

void		set_fds_conn(fd_set *master, int fd)
{
	FD_ZERO(master);
	FD_SET(fd, master);
	FD_SET(1, master);
}
