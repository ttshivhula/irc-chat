/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sclient_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/29 16:02:18 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/30 10:56:19 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

void		add_clients(t_clients **clients, char *name, int client_fd)
{
	t_clients *tmp;

	if (*clients == NULL)
	{
		*clients = (t_clients *)malloc(sizeof(t_clients));
		ft_strcpy((*clients)->nick, name);
		ft_strcpy((*clients)->channel, "#general");
		(*clients)->client_fd = client_fd;
		(*clients)->ring = ring_init();
		(*clients)->next = NULL;
		return ;
	}
	tmp = (t_clients *)malloc(sizeof(t_clients));
	ft_strcpy(tmp->nick, name);
	ft_strcpy(tmp->channel, "#general");
	tmp->client_fd = client_fd;
	tmp->ring = ring_init();
	tmp->next = *clients;
	*clients = tmp;
}

void		remove_client(t_clients **head, int fd)
{
	t_clients	*curr;
	t_clients	*prev;

	prev = NULL;
	curr = *head;
	while (curr != NULL)
	{
		if (curr->client_fd == fd)
		{
			if (prev == NULL)
				*head = curr->next;
			else
				prev->next = curr->next;
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}

void		accept_client(t_server *server)
{
	int						fd;
	struct sockaddr_in		temp;
	socklen_t				socklen;

	socklen = sizeof(struct sockaddr_in);
	ft_bzero(&temp, sizeof(struct sockaddr_in));
	if ((fd = accept((*server).fd, (struct sockaddr *)&temp, &socklen)) == -1)
		ft_putendl("Failed to accept client");
	else
	{
		add_clients(&(*server).clients, ft_strjoin("ttshivhu",
					ft_itoa(fd)), fd);
		broadcast_action(server, server->clients, 1, NULL);
		if (fd > (*server).max_fd)
			(*server).max_fd = fd;
		FD_SET(fd, &((*server).reads));
	}
}
