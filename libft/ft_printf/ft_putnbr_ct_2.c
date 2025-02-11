#include"ft_printf.h"

int	ft_putnbr_ct_2(unsigned int nbr, char *base)
{
	unsigned int	len_base;
	int				ct;

	ct = 0;
	len_base = ft_strlen(base);
	if (nbr < len_base)
		ct += ft_putchar_ct(base[nbr]);
	else
	{
		ct += ft_putnbr_ct_2(nbr / len_base, base);
		ct += ft_putchar_ct(base[nbr % len_base]);
	}
	return (ct);
}
