/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 09:40:49 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 16:03:30 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

int	get_client_fd(t_clients *clients, char *name, int fd)
{
	while (clients)
	{
		if (!ft_strcmp(clients->nick, name))
		{
			if (clients->client_fd == fd)
				return (-1);
			else
				return (clients->client_fd);
		}
		clients = clients->next;
	}
	return (-1);
}

void	client_data(int clientfd, t_server *server)
{
	int		ret;
	char	msg[BUFF_SIZE];
	t_clients *client;

	ft_bzero(msg, sizeof(msg));
	if ((ret = recv(clientfd, msg, sizeof(msg), 0)) > 0)
		read_to_user(server, clientfd, msg);	
	else
	{
		if (ret < 0)
			ft_die("recv failed\n", 1);
		client = get_client(server->clients, clientfd);
		broadcast_action(server, client, 0, NULL);
		remove_client(&(server->clients), clientfd);
		FD_CLR(clientfd, &((*server).reads));
	}
}

static void	server_loop(t_server server)
{
	int			i;
	int			j;
	fd_set		read_fds;

	server.clients  = NULL;
	read_fds = server.reads;
	while (select(server.max_fd + 1, &read_fds, &(server.writes), NULL, NULL) > -1)
	{
		i = -1;
		while (++i < server.max_fd + 1)
		{
			if (FD_ISSET(i, &read_fds))
			{
				i == server.fd ? accept_client(&server) :
					client_data(i, &server);
				j = -1;
				while (++j < server.max_fd + 1)
				{
					if (FD_ISSET(j, &(server.writes)))
						send(j, server.buff, sizeof(server.buff), 0);
				}
				FD_ZERO(&server.writes);
			}
		}
		read_fds = server.reads;
	}
}

int			main(int c, char **v)
{
	struct sockaddr_in	addr;
	t_server			server;

	if (c != 2)
		ft_die("Usage: ./server port\n", 1);
	if ((server.fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_die(ERROR" server socket creation failed\n", 1);
	addr.sin_port = htons(ft_atoi(v[1]));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server.fd, (void*)&(addr), sizeof(addr)) < 0)
		ft_die(ERROR" server port bind failed\n", 1);
	if (listen(server.fd, 1000) == -1)
		ft_die("server failed to listen\n", 1);
	FD_ZERO(&server.reads);
	FD_ZERO(&server.writes);
	FD_SET(server.fd, &(server.reads));
	server.max_fd = server.fd;
	while (42)
		server_loop(server);
	return (0);
}
