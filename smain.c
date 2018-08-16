/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 11:12:34 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/16 14:11:10 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

void	client_data(int listening, int client_fd, int max_fd)
{
	int		size;
	int		i;
	char	buff[4096];

	i = -1;
	if ((size = recv(client_fd, buff, sizeof(buff), 0)) <= 0)
	{
		printf("client: %d left\n", client_fd);
		close(client_fd);
	}
	else
	{
		while (++i <= max_fd)
		{
			if ((i != listening) && (i != client_fd))
				send(i, buff, size, 0);
		}
	}
}

int		init_server(int port)
{
}

int main(int argc, char const *argv[])
{
	int					listening;
	struct sockaddr_in	address;
	int					opt;
	int					addrlen = sizeof(address);

	opt = 1;
	listening = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(atoi(argv[1]));
	bind(listening, (struct sockaddr *)&address, sizeof(address));
	listen(listening, 10000);
	
	fd_set	copy;
	fd_set	master;
	FD_ZERO(&master);
	FD_SET(listening, &master);
	int max;
	int new_sock;

	max = listening;
	while (42)
	{
		copy = master;
		select(max + 1, &copy, NULL, NULL, NULL);
		for (int i = 0; i <= max; i++)
		{
			if (FD_ISSET(i, &copy))
			{
				if (i == listening)
				{
					new_sock = accept(listening, (struct sockaddr *)&address,
							(socklen_t*)&addrlen);
					printf("ready to acept: %d\n", new_sock);
					FD_SET(new_sock, &master);
					max = (new_sock > max) ? new_sock : max;
				}
				else
					client_data(listening, i, max);
			}
		}
	}
	return 0;
}
