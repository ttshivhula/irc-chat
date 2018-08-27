/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 16:25:50 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/27 17:26:20 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

t_clients	*get_client(t_clients *clients, int fd)
{
	while (clients)
	{
		if (clients->client_fd == fd)
			return (clients);
		clients = clients->next;
	}
	return (NULL);
}

void		run_who(t_server *server, char *channel, int fd)
{
	t_clients *tmp;
	char		*buff;
	char		msg[BUFF_SIZE];

	buff = ft_strdup("Users in this channel\n");
	tmp = (*server).clients;
	while (tmp)
	{
		if (!ft_strcmp(tmp->channel, channel))
		{
			buff = ft_strjoin(buff, tmp->username);
			buff = ft_strjoin(buff, "\n");
		}
		tmp = tmp->next;
	}
	buff = ft_strjoin(buff, "\n");
	ft_bzero(msg, sizeof(msg));
	ft_strcpy(msg, buff);
	send(fd, msg, sizeof(msg), 0); 
}

/*
 * TODO: Check if msg is a command, if true run command and then reply to
 * calling user, check if private msg and send to proper user, check if normal
 * add everyone to the write file descriptors then send msg
 */

void		process_msg(t_server *server, t_clients *client, char *msg)
{
	if (!ft_strncmp(msg, "/who\n", 5))
		run_who(server, client->channel, client->client_fd);
	/*else if (!ft_strncmp(msg, "/leave\n", 7) || !ft_strncmp("/join\n", 6))
		channels(server, client->client_fd);*/
}

void		read_to_user(t_server *server, int clientfd, char *buff)
{
	t_clients *client;

	client = get_client((*server).clients, clientfd);
	if (ft_strchr(buff, '\n'))
	{
		ft_strcpy(client->buff + client->offset, buff);
		client->offset = 0;
		//TODO: send to users or process commands...
		printf("full buff\n");
		process_msg(server, client, client->buff);
	}
	else
	{
		ft_strcpy(client->buff + client->offset, buff);
		client->offset = ft_strlen(client->buff);
		printf("part buff\n");
	}
}
