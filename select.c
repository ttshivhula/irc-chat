#include <string.h>

#include <sys/select.h>

int		main(void)
{
	char	buffer[4098];
	int		sel_ret;
	fd_set	readfds;
	struct timeval timing;

	while (42)
	{
		timing.tv_sec = 2;
		timing.tv_usec = 0;
		FD_ZERO(&readfds);
		FD_SET(0, &readfds);
		sel_ret = select(10, &readfds, NULL, NULL, &timing);
		if (sel_ret == 0)
		{
			write(1, "Nothing received\n", 17);
		}
		else
		{
			int ret = read(0, buffer, 4096);
			write(1, buffer, ret);
		}
	}

}
