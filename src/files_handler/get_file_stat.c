#include "../../includes/ft_nm.h"

static struct stat get_file_size (int fd, const char *file)
{
	struct stat file_stat;

	if (fstat (fd, &file_stat) < 0)
	{
		print_error_message ("ft_nm: ", file, ERR_GET_FILE_STATS);
		goto error;
	}
	if (file_stat.st_size == 0)
		goto error;
	if (file_stat.st_size < 0)
	{
		print_error_message ("ft_nm: ", file, ERR_NEGATIVE_SIZE);
		goto error;
	}
	return file_stat;

error:
	close (fd);
	ft_memset (&file_stat, 0, sizeof (struct stat));
	return file_stat;
}

bool get_file_stat (struct stat *file_stat, const char *file, int *fd)
{
	*file_stat = get_file_size (*fd, file);
	if ((*file_stat).st_size == 0)
		return true;

	if (S_ISDIR ((*file_stat).st_mode))
	{
		print_error_message ("ft_nm: Warning : « ", file, ERR_IS_DIR);
		return true;
	}
	else if (!S_ISREG ((*file_stat).st_mode))
	{
		print_error_message ("ft_nm: ", file, ERR_NOT_REGULAR);
		return true;
	}
	else if ((*file_stat).st_size == 0)
		return true;

	return false;
}
