#include "../../includes/ft_nm.h"

static struct stat get_file_size (int fd, const char *file)
{
	struct stat file_stat;

	if (fstat (fd, &file_stat) < 0)
	{
		ft_putstr_fd ("Error getting file stats: ", 2);
		ft_putstr_fd (strerror (errno), 2);
		ft_putstr_fd ("\n", 2);
		goto error;
	}
	if (file_stat.st_size == 0)
		goto error;
	if (file_stat.st_size < 0)
	{
		ft_putstr_fd ("ft_nm: ", 2);
		ft_putstr_fd ((char *)file, 2);
		ft_putstr_fd (": ", 2);
		ft_putstr_fd (ERR_NEGATIVE_SIZE, 2);
		ft_putstr_fd ("\n", 2);
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
		ft_putstr_fd ("ft_nm: Warning : « ", 2);
		ft_putstr_fd ((char *)file, 2);
		ft_putstr_fd (" » is a directory\n", 2);
		return true;
	}
	else if (!S_ISREG ((*file_stat).st_mode))
	{
		ft_putstr_fd ("ft_nm: ", 2);
		ft_putstr_fd ((char *)file, 2);
		ft_putstr_fd (": ", 2);
		ft_putstr_fd (ERR_NOT_REGULAR, 2);
		ft_putstr_fd ("\n", 2);
		return true;
	}
	else if ((*file_stat).st_size == 0)
		return true;

	return false;
}
