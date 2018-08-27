/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 09:40:49 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/27 16:20:19 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

void	add_clients(t_clients **clients, char *name, int client_fd)
{
	t_clients *tmp;

	if (*clients == NULL)
	{
		*clients = (t_clients *)malloc(sizeof(t_clients));
		ft_strcpy((*clients)->username, name);
		ft_strcpy((*clients)->channel, "general");
		(*clients)->client_fd = client_fd;
		(*clients)->offset = 0;
		(*clients)->next = NULL;
		return ;
	}
	tmp = (t_clients *)malloc(sizeof(t_clients));	
	ft_strcpy(tmp->username, name);
	ft_strcpy(tmp->channel, "general");
	tmp->client_fd = client_fd;
	tmp->offset = 0;
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

static void accept_client(t_server *server)
{
	int 					connfd;
	struct sockaddr_in		temp;
	socklen_t				socklen;

	socklen = sizeof(struct sockaddr_in);
	ft_bzero(&temp, sizeof(struct sockaddr_in));
	if ((connfd = accept((*server).fd, (struct sockaddr *)&temp, &socklen)) == -1)
		ft_putendl("Failed to accept client");
	else
	{
		add_clients(&(*server).clients, ft_strjoin("client", ft_itoa(connfd)), connfd);
		if (connfd > (*server).max_fd)
			(*server).max_fd = connfd;
		FD_SET(connfd, &((*server).reads));
	}
}

int	get_client_fd(t_clients *clients, char *name, int fd)
{
	while (clients)
	{
		if (!ft_strcmp(clients->username, name))
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

void	client_data(int clientfd, t_server *server)
{
	int		ret;
	int	i = 0;
	char	msg[BUFF_SIZE];

	ft_bzero(msg, sizeof(msg));
	if ((ret = recv(clientfd, msg, sizeof(msg), 0)) > 0)
	{
		printf("%s", msg);
		/*while (++i <= (*server).max_fd && !buff.type)
		{
			if ((i != (*server).server_fd) && same_channel((*server).clients, i, clientfd))
				(i != clientfd) ? send(i, &buff, sizeof(t_message), 0) : 0;
		}*/
	}
	else
	{
		if (ret < 0)
			ft_die("recv failed\n", 1);
		printf("Client [%d] left server.\n", clientfd);
		FD_CLR(clientfd, &((*server).reads));
	}
}

static void	server_loop(t_server server)
{
	int			i;
	fd_set		read_fds;

	server.clients  = NULL;
	server.channels = NULL;
	add_channels(&server.channels, "general");
	read_fds = server.reads;
	while (select(server.max_fd + 1, &read_fds, NULL, NULL, NULL) > -1)
	{
		printf("max_fd: %d\n", server.max_fd);
		i = 0;
		while (i < server.max_fd + 1)
		{
			if (FD_ISSET(i, &read_fds))
				i == server.fd ? accept_client(&server) :
					client_data(i, &server);
			i++;
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
