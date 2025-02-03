#include "../includes/ft_nm.h"

int		page_size;
bool	is_64;

int main(int argc, const char **argv)
{
	int 		i = 1;
	int			fd;
	char		*default_file = "a.out";
	struct stat	file_stat;
	bool		retval = true;

    page_size = getpagesize();

	if (argc == 1)
	{
		display_files("a.out", 2);
	}
	
	i = 1;
	while (i < argc)
	{
		display_files(argv[i], argc);
		i++;
	}

	exit (retval);
    return retval;
}
