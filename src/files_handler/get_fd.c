#include "../../includes/ft_nm.h"
#include <string.h>

bool get_fd (const char *file, int *fd)
{
	*fd = open (file, O_RDONLY);
	if (*fd < 0)
	{
		print_error_message ("ft_nm: ", file, strerror (errno));
		return true;
	}
	return false;
}
