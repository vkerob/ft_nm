#include "../../includes/ft_nm.h"


bool cleanup_and_report(const char *file, int fd, void *file_map,
	const char *error_msg, size_t map_size)
{
	if (file_map && file_map != MAP_FAILED)
		munmap(file_map, map_size);
	if (fd != -1)
		close(fd);
	if (error_msg[0] == '\0')
		return true;
	ft_putstr_fd("ft_nm: ", 2);
	ft_putstr_fd((char *)file, 2);
	ft_putstr_fd((char *)error_msg, 2);
	return true;
}