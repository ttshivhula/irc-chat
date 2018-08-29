/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 16:25:50 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 15:30:24 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

int		send_command(char *msg, int fd)
{
	char	buff[BUFF_SIZE];

	ft_bzero(buff, sizeof(buff));
	ft_strcpy(buff, ITALIC);
	ft_strcat(buff, GREY);
	ft_strcat(buff, msg);
	ft_strcat(buff, NORMAL);
	ft_strcat(buff, NO_ITALIC);
	send(fd, buff, ft_strlen(buff), 0);
	return (1);
}

int			run_who(t_server *server, t_clients *client)
{
	t_clients *tmp;
	char		buff[BUFF_SIZE];

	ft_bzero(buff, sizeof(buff));
	ft_strcpy(buff, "Active Channel Users:\n");
	tmp = (*server).clients;
	while (tmp)
	{
		if (!ft_strcmp(tmp->channel, client->channel))
		{
			ft_strcat(buff, " -> ");
			ft_strcat(buff, tmp->nick);
			ft_strcat(buff, "\n");
		}
		tmp = tmp->next;
	}
	return send_command(buff, client->client_fd);
}

int		run_nick(t_server *server, t_clients *client)
{
	char	*nick;
	char		buff[BUFF_SIZE];
	t_clients *tmp;

	ft_bzero(buff, sizeof(buff));
	nick = ft_strchr(client->buff, ' ') + 1;
	nick[ft_strlen(nick) - 1] = '\0';
	if (ft_strlen(nick) < 3 || ft_strlen(nick) > 18)
		return send_command("bad nick\n", client->client_fd);
	tmp = (*server).clients;
	while (tmp)
	{
		if (!ft_strcmp(tmp->nick, nick))
			return send_command("nick already in use.\n", client->client_fd);
		tmp = tmp->next;
	}
	broadcast_action(server, client, 0, nick);
	ft_strcpy(client->nick, nick);
	return send_command("nick changed.\n", client->client_fd);
}

int		run_join(t_server *server, t_clients *client)
{
	char	*channel;
	char		buff[BUFF_SIZE];

	ft_bzero(buff, sizeof(buff));
	channel = ft_strchr(client->buff, ' ') + 1;
	channel[ft_strlen(channel) - 1] = '\0';
	if (ft_strlen(channel) < 3 || ft_strlen(channel) > 18)
		return send_command("bad channel.\n", client->client_fd);
	ft_strcpy(client->channel, channel);
	broadcast_action(server, client, 1, NULL);
	return send_command("you joined channel.\n", client->client_fd);
}

int		run_leave(t_server *server, t_clients *client)
{
	broadcast_action(server, client, 2, NULL);
	ft_bzero(client->channel, sizeof(client->channel));
	return send_command("you left channel.\n", client->client_fd);
}

int		group_all_private(t_server *server, t_clients *client, int private, int to_fd)
{
	int	i;

	i = -1;
	ft_bzero(server->buff, sizeof(server->buff));
	ft_strcpy(server->buff, BLUE);
	ft_strcat(server->buff, client->nick);
	private ? ft_strcat(server->buff, RED" [private]") : 0;
	ft_strcat(server->buff, NORMAL" : ");
	ft_strcat(server->buff, client->buff);
	while (++i < server->max_fd + 1 && !private)
	{
		if (same_channel(server->clients, i, client->client_fd))
			FD_SET(i, &(server->writes));
	}
	private ? FD_SET(to_fd, &(server->writes)) : 0;
	return (1);
}

char	*get_nick(char *s)
{
	int	i;
	char	*ret;

	i = 0;
	while (s[i] && s[i] !=  ' ')
		i++;
	if (!i)
		return (NULL);
	ret = malloc(sizeof(char) * i + 1);
	i = 0;
	while (s[i] && s[i] !=  ' ')
	{
		ret[i] = s[i];
		i++;
	}
	ret[i] = '\0';
	return (ret);
}

int		run_msg(t_server *server, t_clients *client)
{
	char	*nick;
	char	*tmp;
	t_clients *user;

	tmp = ft_strchr(client->buff, ' ') + 1;
	nick = get_nick(tmp);
	user = nick ? get_client_nick(server->clients, nick) : NULL;
	user ? ft_strcpy(client->buff, ft_strchr(tmp, ' ') + 1) : 0;
	if (!user || !ft_strcmp(nick, client->nick))
		send_command("unable to send private message.\n", client->client_fd);
	else
		group_all_private(server, client, 1, user->client_fd);
	nick ? free(nick) : 0;
	return (0);
}

t_options	g_options[] = 
{
	{"/join ", 6, run_join},
	{"/leave", 6, run_leave},
	{"/who", 4, run_who},
	{"/nick ", 6, run_nick},
	{"/msg ", 5, run_msg},
	{NULL, 0, NULL}
};

int		process_msg(t_server *server, t_clients *client, char *buff)
{
	int i;

	i = 0;
	while (g_options[i].option)
	{
		if (!ft_strncmp(buff, g_options[i].option, g_options[i].len))
		{
			return (g_options[i].run(server, client));
		}
		i++;
	}
	return (group_all_private(server, client, 0, 0));
}

void		read_to_user(t_server *server, int clientfd, char *buff)
{
	t_clients *client;

	client = get_client((*server).clients, clientfd);
	if (ft_strchr(buff, '\n'))
	{
		ft_strcpy(client->buff + client->offset, buff);
		client->offset = 0;
		process_msg(server, client, client->buff);
	}
	else
	{
		ft_strcpy(client->buff + client->offset, buff);
		client->offset = ft_strlen(client->buff);
	}
}
