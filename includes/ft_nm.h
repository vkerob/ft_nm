# ifndef FT_NM_H

#include "../libft/libft.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
# include <stdbool.h>

extern int	page_size;

typedef struct s_file
{
	char	*name;
	int		fd;
	struct stat file_stat;
}   			t_file;


typedef struct s_data
{
	t_file	*files;
	size_t	file_count;
}   			t_data;



void	read_files(t_data data, int argc, const char **argv);


#endif