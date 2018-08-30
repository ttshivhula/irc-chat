/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   circular.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttshivhu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/30 09:29:40 by ttshivhu          #+#    #+#             */
/*   Updated: 2018/08/30 10:13:24 by ttshivhu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <irc.h>

t_ring*			ring_init(void)
{
	t_ring *tmp;

	tmp = (t_ring *)malloc(sizeof(t_ring));
	tmp->write_ptr = 0;
	tmp->read_ptr = 0;
	ft_bzero(tmp->buff, sizeof(tmp->buff));
	return (tmp);
}

void		ft_write(t_ring **ring, char *buff)
{
	char	*tmp;

	tmp = buff;
	while (*tmp)
	{
		if (((*ring)->write_ptr + 1) % BUFF_SIZE != (*ring)->read_ptr)
		{
			(*ring)->buff[(*ring)->write_ptr] = *tmp++;
			(*ring)->write_ptr = ((*ring)->write_ptr + 1) % BUFF_SIZE;
		}
		else
			break ;
	}
}

static int	ft_get(t_ring **ring, char	*data)
{
    int r = 0;

    if((*ring)->read_ptr != (*ring)->write_ptr)
    {
        *data = (*ring)->buff[(*ring)->read_ptr];
		(*ring)->read_ptr = ((*ring)->read_ptr + 1) % BUFF_SIZE;
        r = 1;
    }
    return (r);
}

void		ft_read(t_ring **ring, char *str)
{
	int		i;
	char	data;

	i = 0;
	while (ft_get(ring, &data))
		str[i++] = data;
	str[i] = '\0';
}
