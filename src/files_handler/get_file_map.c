#include "../../includes/ft_nm.h"

bool get_file_map (void **file_map, struct stat file_stat, const char *file,
				   int fd)
{
	*file_map = mmap (NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (*file_map == MAP_FAILED)
	{
		return cleanup_and_report (file, fd, file_map, ERR_MAP_FAILED,
								   file_stat.st_size);
	}

	return false;
}