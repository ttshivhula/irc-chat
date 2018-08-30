/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 14:49:27 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/30 08:32:15 by ttshivhu         ###   ########.fr       */
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

int		group_all_private(t_server *server, t_clients *client, int private, int to_fd)
{
	int	i;

	i = -1;
	(void)to_fd;
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
	if (private)
		FD_SET(to_fd, &(server->writes));
	return (1);
}

int		broadcast_action(t_server *server, t_clients *client, int action, char *nick)
{
	int	i;

	i = -1;
	ft_bzero(server->buff, sizeof(server->buff));
	ft_strcpy(server->buff, ITALIC);
    ft_strcat(server->buff, GREY);
	ft_strcat(server->buff, client->nick);
	ft_strcat(server->buff, " ");
    (!action && !nick) ? ft_strcat(server->buff, " left chat\n") : 0;
    (action == 1) ? ft_strcat(server->buff, " joined group\n") : 0;
    (action == 2) ? ft_strcat(server->buff, " left group\n") : 0;
    (nick) ? ft_strcat(server->buff, " changed nick to ") : 0;
    (nick) ? ft_strcat(server->buff, nick) : 0;
    (nick) ? ft_strcat(server->buff, "\n") : 0;
    ft_strcat(server->buff, NORMAL);
    ft_strcat(server->buff, NO_ITALIC);
	while (++i < server->max_fd + 1)
	{
		if (same_channel(server->clients, i, client->client_fd))
			FD_SET(i, &(server->writes));
	}
	return (1);
}
