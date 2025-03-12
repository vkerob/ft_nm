#include "../../includes/ft_nm.h"
#include <string.h>

bool get_fd (const char *file, int *fd)
{
	*fd = open (file, O_RDONLY);
	if (*fd < 0)
	{
		ft_putstr_fd ("ft_nm: ", 2);
		ft_putstr_fd ((char *)file, 2);
		ft_putstr_fd (": ", 2);
		ft_putstr_fd (strerror (errno), 2);
		ft_putstr_fd ("\n", 2);

		return true;
	}
	return false;
}
