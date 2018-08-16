/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   smain.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 11:12:34 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/16 15:30:46 by ttshivhu         ###   ########.fr       */
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
	int					listening;
	struct sockaddr_in	address;
	int					opt;
	int					error;

	error = 0;
	opt = 1;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	if ((listening = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		error++;
	if ((bind(listening, (struct sockaddr *)&address, sizeof(address)) == -1))
		error++;
	if (listen(listening, 10000) == -1)
		error++;
	return (error ? 0 : listening);
}

int main(int c, char const **v)
{
	int					listening;
	struct sockaddr_in	address;
	int					addrlen = sizeof(address);
	fd_set				copy;
	fd_set				master;
	fd_set				copyr;
	fd_set				masterr;
	int					max;
	int					new_sock;

	if (c == 2)
	{
		if (!(listening = init_server(atoi(v[1]))))
			printf("Failed to start server\n");
	}
	else
		exit(1);
	FD_ZERO(&master);
	FD_SET(listening, &master);
	FD_ZERO(&masterr);
	FD_SET(listening, &masterr);
	max = listening;
	while (42)
	{
		copy = master;
		copyr = masterr;
		select(max + 1, &copy, &copyr, NULL, NULL);
		for (int i = 0; i <= max; i++)
		{
			if (FD_ISSET(i, &copy) || FD_ISSET(i, &copyr))
			{
				if (i == listening)
				{
					new_sock = accept(listening, (struct sockaddr *)&address,
							(socklen_t*)&addrlen);
					printf("new client connected: %d\n", new_sock);
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
