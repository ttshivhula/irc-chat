/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   generic.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 10:34:23 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/30 10:35:06 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

void		ft_die(char *str, int exit_code)
{
	write(2, str, ft_strlen(str));
	exit(exit_code);
}

char		*get_client_channel(t_clients *clients, int fd)
{
	while (clients)
	{
		if (clients->client_fd == fd)
			return (clients->channel);
		clients = clients->next;
	}
	return (NULL);
}

int			same_channel(t_clients *clients, int client_one, int client_two)
{
	char	*s1;
	char	*s2;

	s1 = get_client_channel(clients, client_one);
	s2 = get_client_channel(clients, client_two);
	if (!s1 || !s2)
		return (0);
	if (!ft_strcmp(s1, s2) && (client_one != client_two))
		return (1);
	return (0);
}

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

t_clients	*get_client_nick(t_clients *clients, char *nick)
{
	while (clients)
	{
		if (!ft_strcmp(clients->nick, nick))
			return (clients);
		clients = clients->next;
	}
	return (NULL);
}
