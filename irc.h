#ifndef IRC_H
# define IRC_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <sys/param.h>
# include <stdint.h>
# include <sys/wait.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <dirent.h>
# include <libft.h>

# define BUFF_SIZE 4096
# define NAME_SIZE 20

# define ERROR "\x1b[31mERROR:\x1b[0m"
# define SUCCESS "\x1b[32mSUCCESS:\x1b[0m"

typedef	struct			s_clients
{
	char			username[NAME_SIZE];
	int			client_fd;
	char			channel[NAME_SIZE];
	struct s_clients	*next;
}				t_clients;

typedef struct			s_channels
{
	char			name[NAME_SIZE];
	struct	s_channels	*next;
}				t_channels;

typedef struct			s_message
{
	char			msg[BUFF_SIZE];
	char			from[NAME_SIZE];
	char			to[NAME_SIZE];
	int			type; // 0 - normal, 1 - join, 2 - private
}				t_message;

typedef struct			s_server
{
	t_channels		*channels;
	t_clients		*clients;
	int			server_fd;
}				t_server;

void	ft_die(char *str, int exit_code);

# endif
