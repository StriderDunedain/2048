/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetutsc <dpetutsc@student.42vienna.at>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 14:04:30 by dpetutsc          #+#    #+#             */
/*   Updated: 2026/11/19 14:04:30 by dpetutsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>
#include <unistd.h>

// UTF-8 stores the leading marker bits (1-5) at the start of the first byte
// and the marker "10" at the start of every following byte (up to 4 total).
// e.g. U+2500
//      xxxx0010  xx010100  xx000000
//      │         │         └─ lowest 6 bits
//      │         └───────── middle 6 bits
//      └──────────────── top 4 bits
// The leading 1s in the first byte tell the reader how many bytes the
// character has.
// e.g. 0 = 1 byte, 110 = 2 bytes, 1110 = 3 bytes, 11110 = 4 bytes
// So we shift the bits into the right place to convert a single wchar_t
// (int) into UTF-8, in order to use wide-chars without using any
// disallowed functions.
static int	wchar_to_utf8(wchar_t c, unsigned char *buf)
{
	int	len;

	if (!buf)
		return (0);
	len = 1 + (c >= 0x80) + (c >= 0x800) + (c >= 0x10000);
	if (len == 1)
		buf[0] = c;
	else if (len == 2)
		buf[0] = 0xC0 | (c >> 6);
	else if (len == 3)
		buf[0] = 0xE0 | (c >> 12);
	else
		buf[0] = 0xF0 | (c >> 18);
	if (len >= 2)
		buf[len - 1] = 0x80 | (c & 0x3F);
	if (len >= 3)
		buf[len - 2] = 0x80 | ((c >> 6) & 0x3F);
	if (len == 4)
		buf[1] = 0x80 | ((c >> 12) & 0x3F);
	return (len);
}

int	ft_putwstr_fd(wchar_t *str, int fd)
{
	unsigned char	*buf;
	size_t			len;
	size_t			pos;
	ssize_t			written;

	if (!str)
		return (EXIT_FAILURE);
	buf = malloc(ft_wstrlen(str) * 5 + 1);
	if (!buf)
		return (EXIT_FAILURE);
	len = 0;
	while (*str)
	{
		if (*str == L'\n')
			buf[len++] = '\r';
		len += wchar_to_utf8(*str++, &buf[len]);
	}
	pos = 0;
	while (pos < len)
	{
		written = write(fd, buf + pos, len - pos);
		if (written <= 0)
			return (free(buf), EXIT_FAILURE);
		pos += (size_t)written;
	}
	free(buf);
	return (EXIT_SUCCESS);
}
