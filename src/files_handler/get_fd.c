#include "../../includes/ft_nm.h"


bool	get_fd(const char *file, int *fd)
{
	*fd = open(file, O_RDONLY);
	if (*fd < 0)
	{
		if (errno == ENOENT)
			ft_printf("ft_nm : « %s »: No such file\n", file);
		close(*fd);
		return true;
	}
	return false;
}
