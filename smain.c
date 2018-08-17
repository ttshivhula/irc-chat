/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 09:40:49 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/15 10:48:05 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

void	add_clients(t_clients **clients, char *name, int client_fd)
{
	t_clients *tmp;

	printf("Adding client [%d] -- [%s] to chat\n", client_fd, name);	
	if (*clients == NULL)
	{
		*clients = (t_clients *)malloc(sizeof(t_clients));
		ft_strcpy((*clients)->username, name);
		ft_strcpy((*clients)->channel, "general");
		(*clients)->client_fd = client_fd;
		(*clients)->next = NULL;
		return ;
	}
	tmp = (t_clients *)malloc(sizeof(t_clients));	
	ft_strcpy(tmp->username, name);
	ft_strcpy(tmp->channel, "general");
	tmp->client_fd = client_fd;
	tmp->next = *clients;
	*clients = tmp;
}

void	add_channels(t_channels **channels, char *name)
{
	t_channels *tmp;
	
	if (*channels == NULL)
	{
		*channels = (t_channels *)malloc(sizeof(t_channels));
		ft_strcpy((*channels)->name, name);
		(*channels)->next = NULL;
		return ;
	}
	tmp = (t_channels *)malloc(sizeof(t_channels));	
	ft_strcpy(tmp->name, name);
	tmp->next = *channels;
	*channels = tmp;
}

static void accept_client(int server_fd, fd_set *master, int *fd_max)
{
	int 					connfd;
	struct sockaddr_in		temp;
	socklen_t				socklen;

	socklen = sizeof(struct sockaddr_in);
	ft_bzero(&temp, sizeof(struct sockaddr_in));
	if ((connfd = accept(server_fd, (struct sockaddr *)&temp, &socklen)) == -1)
		ft_putendl("Failed to accept client");
	else
	{
		if (connfd > *fd_max)
			*fd_max = connfd;
		//printf("New client [%d] joined.\n", connfd);
		FD_SET(connfd, master);
	}
}

int	get_client_fd(t_clients *clients, char *name)
{
	while (clients)
	{
		if (!ft_strcmp(clients->username, name))
			return (clients->client_fd);
		clients = clients->next;
	}
	return (-1);
}

char	*get_client_channel(t_clients *clients, int fd)
{
	while (clients)
	{
		if (clients->client_fd == fd)
			return (clients->channel);
		clients = clients->next;
	}
	return (NULL);
}

int	same_channel(t_clients *clients, int client_one, int client_two)
{
	char	*s1;
	char	*s2;

	s1 = get_client_channel(clients, client_one);
	s2 = get_client_channel(clients, client_two);
	if (!s1 || !s2)
		return (0);
	if (!ft_strcmp(s1, s2))
		return (1);
	return (0);
}

void	client_data(int clientfd, fd_set *master, int max_fd, t_server *server)
{
	int		ret;
	int	i = 0;
	t_message	buff;

	ft_bzero(&buff, sizeof(t_message));
	if ((ret = recv(clientfd, &buff, sizeof(t_message), 0)) > 0)
	{
		//printf("type: %d to: %s from: %s mesg: %s\n", buff.type, buff.to, buff.from, buff.msg);
		(buff.type == 1) ? add_clients(&((*server).clients), buff.from, clientfd) : 0;
		(buff.type == 2) ? send(get_client_fd((*server).clients, buff.to), &buff,
		sizeof(buff), 0) : 0;
		while (++i <= max_fd && !buff.type)
		{
			if ((i != (*server).server_fd) && same_channel((*server).clients, i, clientfd))
				(i != clientfd) ? send(i, &buff, sizeof(t_message), 0) : 0;
		}
	}
	else
	{
		if (ret < 0)
			ft_die("recv failed\n", 1);
		printf("Client [%d] left server.\n", clientfd);
		FD_CLR(clientfd, master);
	}
}

static void	server_loop(fd_set master, int max_fd, int server_fd)
{
	int		i;
	fd_set	select_fds;
	t_server	server;

	server.clients  = NULL;
	server.channels = NULL;
	add_channels(&server.channels, "general");
	select_fds = master;
	server.server_fd = server_fd;
	while (select(max_fd + 1, &select_fds, NULL, NULL, NULL) > -1)
	{
		i = 0;
		while (i < max_fd + 1)
		{
			if (FD_ISSET(i, &select_fds))
				i == server_fd ? accept_client(server_fd, &master, &max_fd) :
					client_data(i, &master, max_fd, &server);
			i++;
		}
		select_fds = master;
	}
}

int			main(int c, char **v)
{
	struct sockaddr_in	addr;
	int					server_fd;
	fd_set		master;
	int		fd_max;

	if (c != 2)
		ft_die("Usage: ./server port\n", 1);
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_die(ERROR" server socket creation failed\n", 1);
	addr.sin_port = htons(ft_atoi(v[1]));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(server_fd, (void*)&(addr), sizeof(addr)) < 0)
		ft_die(ERROR" server port bind failed\n", 1);
	if (listen(server_fd, 1000) == -1)
		ft_die("server failed to listen\n", 1);
	FD_ZERO(&master);
	FD_SET(server_fd, &master);
	fd_max = server_fd;
	while (42)
		server_loop(master, fd_max, server_fd);
	return (0);
}
