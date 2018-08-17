/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 09:00:29 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/15 12:07:28 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

char		*private_msg(char *buff)
{
	char	*ptr;
	int	i;
	
	i = 0;
	while (*buff && i != 2)
	{
		ptr = buff;
		if (*ptr == ' ')
			i++;
		buff++;
	}
	return (ptr);
}

int		send_message_to_server(int sockfd, char *username)
{
	t_message		msg;
	char			**ptr;

	ft_bzero(&msg, sizeof(t_message));
	msg.type = 0;
	ft_strcpy(msg.from, username);
	read(1, msg.msg, 4096);
	ptr = ft_strsplit(msg.msg, ' ');
	if (ptr[0] && ptr[1] && ptr[2])
	{
		if (!ft_strcmp(ptr[0], "/msg"))
		{
			ft_strcpy(msg.to, ptr[1]);
			ft_strcpy(msg.msg, private_msg(msg.msg));
			msg.type = 2;
		}
	}
	if (send(sockfd, &msg, sizeof(t_message), 0) == -1)
		return (0);
	return (EXIT_SUCCESS);
}

int		recv_message_from_server(int sockfd)
{
	int			ret;
	t_message		msg;

	ft_bzero(&msg, sizeof(t_message));
	if ((ret = recv(sockfd, &msg, sizeof(t_message), 0)) < 1)
	{
		printf("Server quit, try again later!\n");
		return (EXIT_FAILURE);
	}
	if (!msg.type)
		printf("\x1b[36m%s\x1b[0m : %s", msg.from, msg.msg);
	else if (msg.type == 2)
		printf("\x1b[36m%s\x1b[35m [DM]\x1b[0m : %s", msg.from, msg.msg);
	else
		printf("%s", msg.msg);
	return (EXIT_SUCCESS);
}


static int 	client_loop(int sockfd, int max_fd, fd_set master, char *username)
{
	fd_set		select_fds;
	int			i;
	
	select_fds = master;
	while (select(FD_SETSIZE, &select_fds, NULL, NULL, NULL) > -1)
	{
		i = 0;
		while (i < FD_SETSIZE)
		{
			if (FD_ISSET(i, &select_fds))
			{
				if (i == 1)
					send_message_to_server(sockfd, username);
				else if (recv_message_from_server(sockfd) == EXIT_FAILURE)
					return (EXIT_FAILURE);
			}
			i++;
		}
		select_fds = master;
	}
	close(sockfd);
	return (0);
}

void			send_username(char *username, int server_fd)
{
	t_message		connect_msg;

	ft_bzero(&connect_msg, sizeof(t_message));
	connect_msg.type = 1;
	ft_strcpy(connect_msg.from, username);
	send(server_fd, &connect_msg, sizeof(t_message), 0);
}

int			main(int c, char **v)
{
	struct sockaddr_in	addr;
	struct hostent		*host;
	int					fd;
	fd_set master;

	if (c != 4)
		ft_die("Usage: ./client host port\n", 1);
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		ft_die(ERROR" client socket creation failed\n", 1);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(ft_atoi(v[2]));
	if ((host = gethostbyname(v[1])) == NULL)
		ft_die(ERROR" check your host\n", 1);
	ft_memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);
	if (connect(fd, (void*)&(addr), sizeof(addr)) < 0)
		ft_die(ERROR" unable to connect\n", 1);
	FD_ZERO(&master);
	FD_SET(fd, &master);
	FD_SET(1, &master);
	send_username(v[3], fd);
	client_loop(fd, fd, master, v[3]);
	return (0);
}
