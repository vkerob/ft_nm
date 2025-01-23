# include "../includes/ft_nm.h"

static void write_hex_64bytes(void *data, int fd_stdout)
{
    static const char hexdig[] = "0123456789abcdef";
    unsigned char *p = (unsigned char*)data;

    for (int i = 0; i < 64; i++) {
        char buf[3];
        buf[0] = hexdig[(p[i] >> 4) & 0xF];
        buf[1] = hexdig[p[i] & 0xF];
        buf[2] = ' ';
        write(fd_stdout, buf, 3);
    }
    write(fd_stdout, "\n", 1);
}


void print_memory(const void *header, size_t size) {
    const unsigned char *bytes = (const unsigned char *)header;
    size_t i;

    for (i = 0; i < size; i++) {
        printf("%02x ", bytes[i]);
        if ((i + 1) % 16 == 0) { // Saut de ligne tous les 16 octets
            printf("\n");
        }
    }
    if (i % 16 != 0) {
        printf("\n");
    }
}

void ft_print_memory(const void *header, size_t size) {
    const unsigned char *bytes = (const unsigned char *)header;
    size_t i;

    for (i = 0; i < size; i++) {
        if (bytes[i] < 16) {
            ft_printf("0%x ", bytes[i]); // Ajoute un zéro pour les nombres inférieurs à 16
        } else {
            ft_printf("%x ", bytes[i]);
        }
        if ((i + 1) % 16 == 0) { // Saut de ligne tous les 16 octets
            ft_printf("\n");
        }
    }
    if (i % 16 != 0) {
        ft_printf("\n"); // Saut de ligne final si nécessaire
    }
}


int	open_file(const char *file)
{
	int			fd;
	struct stat	file_stat;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("ft_nm :%s: %s\n", file, strerror(errno));
		return -1;
	}
	return fd;
}


struct stat	get_file_size(int fd, const char *file)
{
	struct stat	file_stat;

	if (fstat(fd, &file_stat) < 0)
	{
		perror("Error getting file stats");
		close(fd);
		ft_memset(&file_stat, 0, sizeof(struct stat));
	}
	else if (file_stat.st_size == 0)
	{
		// do nothing if the file is empty
		close(fd);
		ft_memset(&file_stat, 0, sizeof(struct stat));
	}
	else if (file_stat.st_size < 0)
	{
		ft_printf("ft_nm :%s: has negative size, probably it is too large\n", file);
		close(fd);
		ft_memset(&file_stat, 0, sizeof(struct stat));

	}
	else if (file_stat.st_size < sizeof(Elf64_Ehdr))
	{
		ft_printf("ft_nm : %s: File format not recognized\n", file);
		close(fd);
		ft_memset(&file_stat, 0, sizeof(struct stat));
	}
	return file_stat;
}

bool	display_files(const char *file)
{
	int			i;
	int			fd;
	struct stat	file_stat;

	fd = open_file(file);
	if (fd > -1)
	{
		file_stat = get_file_size(fd, file);
		if (file_stat.st_size == 0)
			return true;
	}
	else
	{
		if (errno == ENOENT)
			ft_printf("ft_nm :%s: No such file\n", file);
		else if (S_ISDIR(file_stat.st_mode))
			ft_printf("ft_nm :%s: is a directory\n", file);
		else if (!S_ISREG(file_stat.st_mode))
			ft_printf("ft_nm :%s: is not an ordinary file\n", file);
		close(fd);
		return true;
	}
	// read 64 bytes from the file for being sure it's an ELF file
	// if all the file have been already mapped, we can use the mapping
	// else we map the file

	return false;
}
