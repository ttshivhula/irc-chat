/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 14:07:37 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/16 14:41:13 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	IRC_H
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
# include <netinet/in.h>
# include <arpa/inet.h>
# include <netdb.h>
# include <dirent.h>

typedef	struct			s_channels
{
	char				*username;
	struct s_channels	*next;
}						t_channels;

typedef struct			s_user
{
	char				*username;
	char				*channel;
}						t_user;

#endif
