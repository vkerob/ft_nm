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


void	display_files(const char *file);


#endif