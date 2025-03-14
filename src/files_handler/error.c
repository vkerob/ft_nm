#include "../../includes/ft_nm.h"

void print_error_message (const char *prefix, const char *file, const char *msg)
{
	ft_putstr_fd ((char *)prefix, 2);
	ft_putstr_fd ((char *)file, 2);
	if (ft_strcmp (msg, ERR_IS_DIR) == 0)
		ft_putstr_fd (" » ", 2);
	else
		ft_putstr_fd (": ", 2);
	ft_putstr_fd ((char *)msg, 2);
}

bool cleanup_and_report (const char *file, int fd, void *file_map,
						 const char *error_msg, size_t map_size)
{
	if (file_map && file_map != MAP_FAILED)
		munmap (file_map, map_size);
	if (fd != -1)
		close (fd);
	if (error_msg[0] != '\0')
		print_error_message ("ft_nm: ", file, error_msg);
	return true;
}