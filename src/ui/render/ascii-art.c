/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ascii-art.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dpetutsc <dpetutsc@student.42vienna.at>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/19 14:04:30 by dpetutsc          #+#    #+#             */
/*   Updated: 2026/11/19 14:04:30 by dpetutsc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "render.h"
#include <stdlib.h>

wchar_t	*get_ascii_art_digit(int nb)
{
	if (nb == 0)
		return (L"┌─┐│ │└─┘");
	else if (nb == 1)
		return (L"  ╷  │  ╵");
	else if (nb == 2)
		return (L"╶─┐┌─┘└─╴");
	else if (nb == 3)
		return (L"╶─┐╶─┤╶─┘");
	else if (nb == 4)
		return (L"╷ ╷└─┤  ╵");
	else if (nb == 5)
		return (L"┌─╴└─┐╶─┘");
	else if (nb == 6)
		return (L"┌─╴├─┐└─┘");
	else if (nb == 7)
		return (L"╶─┐  │  ╵");
	else if (nb == 8)
		return (L"┌─┐├─┤└─┘");
	else if (nb == 9)
		return (L"┌─┐└─┤╶─┘");
	return (NULL);
}
