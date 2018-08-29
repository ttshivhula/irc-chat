/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 09:00:29 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 18:23:16 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

int		send_message_to_server(int *sockfd, fd_set *master)
{
	char			msg[BUFF_SIZE];
	struct sockaddr_in addr;
	struct hostent *host;

	host = (struct hostent *)0;
	addr.sin_family = AF_INET;
	ft_bzero(msg, sizeof(msg));
	read(1, msg, 4096);
	if (!ft_strcmp(msg, "/exit\n"))
		exit(0);
	if (!ft_strncmp(msg, "/connect", 8))
	{
		close(*sockfd);
		*sockfd = socket(AF_INET, SOCK_STREAM, 0);
		in_server_connection(addr, host, msg, sockfd);
		set_fds_conn(master, *sockfd);
	}
	else
	{
		if (send(*sockfd, msg, sizeof(msg), 0) == -1)
			return (0);
	}
	return (EXIT_SUCCESS);
}

int		recv_message_from_server(int sockfd)
{
	int			ret;
	char			msg[BUFF_SIZE];

	ft_bzero(msg, sizeof(msg));
	if ((ret = recv(sockfd, msg, sizeof(msg), 0)) < 1)
	{
		ft_putstr("lost server connection.\n");
		return (EXIT_FAILURE);
	}
	ft_putstr(msg);
	return (EXIT_SUCCESS);
}


static int 	client_loop(int *sockfd, fd_set *master)
{
	fd_set		select_fds;
	int			i;
	
	select_fds = *master;
	while (select(FD_SETSIZE, &select_fds, NULL, NULL, NULL) > -1)
	{
		i = 0;
		while (i < FD_SETSIZE)
		{
			if (FD_ISSET(i, &select_fds))
			{
				if (i == 1)
					send_message_to_server(sockfd, master);
				else if (recv_message_from_server(*sockfd) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
			i++;
		}
		select_fds = *master;
	}
	close(*sockfd);
	return (0);
}

void	connect_cmd(struct sockaddr_in addr, struct hostent	*host, int fd)
{
	char				msg[BUFF_SIZE];
	int					ret;
	char				**parts;

	ret = -1;	
	while (ret == -1)
	{
		printf("Unable to connect, please connect to server\n");
		ft_bzero(msg, sizeof(msg));
		ret = read(0, msg, sizeof(msg));
		msg[ret] = '\0';
		ret = -1;
		if (ft_strncmp("/connect", msg, 8) == 0)
		{
			parts = ft_strsplit(msg, ' ');
			addr.sin_port = htons(ft_atoi(parts[2]));
			host = gethostbyname(parts[1]);
			ft_memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);
			ret = connect(fd, (void*)&(addr), sizeof(addr));
		}
	}
}

int			main(int c, char **v)
{
	struct sockaddr_in	addr;
	struct hostent		*host;
	int					fd;
	fd_set master;

	addr.sin_family = AF_INET;
	host = NULL;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (c != 3)
		connect_cmd(addr, host, fd);
	else
	{
		if ((host = gethostbyname(v[1])) == NULL)
			ft_die(ERROR" check your host\n", 1);
		addr.sin_port = htons(ft_atoi(v[2]));
		ft_memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);
		if (connect(fd, (void*)&(addr), sizeof(addr)) < 0)
			ft_die(ERROR" unable to connect\n", 1);
	}
	set_fds_conn(&master, fd);
	client_loop(&fd, &master);
	return (0);
}
