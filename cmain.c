/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 09:00:29 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/27 15:45:12 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

int		send_message_to_server(int sockfd)
{
	char			msg[BUFF_SIZE];
	
	ft_bzero(&msg, sizeof(msg));
	read(1, msg, 4096);
	if (send(sockfd, msg, sizeof(msg), 0) == -1)
		return (0);
	return (EXIT_SUCCESS);
}

int		recv_message_from_server(int sockfd)
{
	int			ret;
	char			msg[BUFF_SIZE];

	ft_bzero(&msg, sizeof(msg));
	if ((ret = recv(sockfd, &msg, sizeof(msg), 0)) < 1)
	{
		printf("Server quit, try again later!\n");
		return (EXIT_FAILURE);
	}
	printf("%s", msg);
	return (EXIT_SUCCESS);
}


static int 	client_loop(int sockfd, int max_fd, fd_set master)
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
					send_message_to_server(sockfd);
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

int			main(int c, char **v)
{
	struct sockaddr_in	addr;
	struct hostent		*host;
	int					fd;
	fd_set master;

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
	client_loop(fd, fd, master);
	return (0);
}
