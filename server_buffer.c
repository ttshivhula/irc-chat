/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_buffer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/27 16:25:50 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/29 15:51:30 by ttshivhu         ###   ########.fr       */
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
