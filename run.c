/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 15:47:56 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 15:49:24 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

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
	if (ft_strlen(nick) < 3 || ft_strlen(nick) > 9)
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
	if (ft_strlen(channel) < 3 || ft_strlen(channel) > 9)
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
