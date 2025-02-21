#include "../../includes/ft_nm.h"

static struct stat get_file_size(int fd, const char *file)
{
    struct stat file_stat;

    if (fstat(fd, &file_stat) < 0)
    {
        perror("Error getting file stats");
        goto error;
    }
    if (file_stat.st_size == 0)
    {
        goto error;
    }
    if (file_stat.st_size < 0)
    {
        ft_printf("ft_nm : %s: has negative size, probably it is too large\n",
                  file);
        goto error;
    }
    if (file_stat.st_size < sizeof(Elf64_Ehdr))
    {
        ft_printf("ft_nm : %s: File format not recognized\n", file);
        goto error;
    }
    return file_stat;

error:
    close(fd);
    ft_memset(&file_stat, 0, sizeof(struct stat));
    return file_stat;
}

bool get_file_stat(struct stat *file_stat, const char *file, int *fd)
{
    *file_stat = get_file_size(*fd, file);
    if (S_ISDIR((*file_stat).st_mode))
    {
        ft_printf("ft_nm :%s: is a directory\n", file);
        return true;
    }
    else if (!S_ISREG((*file_stat).st_mode))
    {
        ft_printf("ft_nm :%s: is not an ordinary file\n", file);
        return true;
    }
    else if ((*file_stat).st_size == 0)
        return true;
    return false;
}