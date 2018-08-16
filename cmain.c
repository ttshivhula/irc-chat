/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 09:00:29 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/16 15:26:49 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

static void	irc_receive(int sock_fd)
{
	char buff[4096];

	size_t c = recv(sock_fd, buff, 4096, 0);
	write(1, buff, c);
}

static void	irc_write(int sock_fd)
{
	char	buff[4096];
	int		sent;

	sent = read(0, buff, 4096);
	send(sock_fd, buff, sent, 0);
}

void		client_main_loop(int sock_fd)
{
	fd_set			master;
	fd_set			copy;
	struct timeval	timing;
	int				sel_ret;

	FD_ZERO(&master);
	FD_SET(0, &master);
	while (42)
	{
		copy = master;
		timing.tv_sec = 1;
		timing.tv_usec = 0;
		sel_ret = select(1, &copy, NULL, NULL, &timing);
		if (sel_ret != 0)
			irc_write(sock_fd);
		irc_receive(sock_fd);
	}
}

int			main(int c, char **v)
{
	struct sockaddr_in	addr;
	struct hostent		*host;
	int					fd;

	if (c != 3)
		exit(1);
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		exit(1);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(v[2]));
	if ((host = gethostbyname(v[1])) == NULL)
		exit(1);
	memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);
	if (connect(fd, (void*)&(addr), sizeof(addr)) < 0)
		exit(1);
	client_main_loop(fd);
	return (0);
}
