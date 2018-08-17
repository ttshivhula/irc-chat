#include <irc.h>

void	ft_die(char *str, int exit_code)
{
	write(2, str, ft_strlen(str));
	exit(exit_code);
}
