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
# define NORMAL			"\x1B[0m"
# define RED			"\x1B[31m"
# define GREEN			"\x1B[32m"
# define YELLOW			"\x1B[33m"
# define BLUE			"\x1B[34m"
# define MAGNETA		"\x1B[35m"
# define CYAN			"\x1B[36m"
# define WHITE			"\x1B[37m"
# define PINK			"\033[38;5;200m"
# define ORANGE			"\033[38;5;208m"
# define PURPLE			"\033[38;5;55m"
# define MAROON			"\033[38;5;88m"
# define GREY			"\033[38;5;246m"

# define BOLD			"\033[1m"
# define WHATISIT		"\033[2m"
# define ITALIC			"\033[3m"
# define UNDERLINE 		"\033[4m"
# define CROSSED 		"\033[9m"
# define NO_BOLD		"\033[21m"
# define NO_ITALIC		"\033[23m"
# define NO_UNDERLINE 	"\033[24m"
# define NO_CROSSED 	"\033[29m"

typedef	struct			s_clients
{
	char				username[NAME_SIZE];
	char				buff[BUFF_SIZE];
	int					offset;
	int					client_fd;
	char				channel[NAME_SIZE];
	struct s_clients	*next;
}						t_clients;

typedef struct			s_channels
{
	char				name[NAME_SIZE];
	struct s_channels	*next;
}						t_channels;

typedef struct			s_server
{
	t_channels			*channels;
	t_clients			*clients;
	int					fd;
	int					max_fd;
	fd_set				reads;
	fd_set				writes;
}						t_server;

void					ft_die(char *str, int exit_code);
void		read_to_user(t_server *server, int clientfd, char *buff);

# endif
