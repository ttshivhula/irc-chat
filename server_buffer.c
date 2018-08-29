/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 16:25:50 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 19:09:41 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>


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

int		process_msg(t_server *server, t_clients *client, char *buff)
{
	if (!ft_strncmp(buff, "/join ", 6))
		return run_join(server, client);
	if (!ft_strncmp(buff, "/leave", 6))
		return run_leave(server, client);
	if (!ft_strncmp(buff, "/who", 4))
		return run_who(server, client);
	if (!ft_strncmp(buff, "/nick ", 6))
		return run_nick(server, client);
	if (!ft_strncmp(buff, "/msg ", 5))
		return run_msg(server, client);
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
