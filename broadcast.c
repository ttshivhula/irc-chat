/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   broadcast.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 14:49:27 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 15:43:08 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

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
