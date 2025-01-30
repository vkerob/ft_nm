# include "../includes/ft_nm.h"

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


bool	get_fd(const char *file, int *fd)
{
	*fd = open_file(file);
	if (*fd < 0)
	{
		if (errno == ENOENT)
			ft_printf("ft_nm :%s: No such file\n", file);
		close(*fd);
		return true;
	}
	return false;
}


bool	get_file_stat(struct stat *file_stat, const char *file, int *fd)
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


bool	check_header(void **file_map, struct stat file_stat, const char *file, int fd)
{
	*file_map = mmap(NULL, page_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (*file_map == MAP_FAILED)
	{
		perror("Error mapping file");
		close(fd);
		return false;
	}
	unsigned char *e_ident = (unsigned char *)(*file_map);
    if (e_ident[EI_MAG0] != ELFMAG0 ||
        e_ident[EI_MAG1] != ELFMAG1 || 
        e_ident[EI_MAG2] != ELFMAG2 || 
        e_ident[EI_MAG3] != ELFMAG3)
    {
        ft_printf("ft_nm :%s: File format not recognized\n", file);
        close(fd);
		munmap(*file_map, page_size);
        return true;
    }

    if (e_ident[EI_CLASS] == ELFCLASS64)
    {
        Elf64_Ehdr *header = (Elf64_Ehdr *)(*file_map);
        if (header->e_type != ET_EXEC && header->e_type != ET_DYN && header->e_type != ET_REL)
        {
            ft_printf("ft_nm :%s: File format not recognized\n", file);
            close(fd);
			munmap(*file_map, page_size);
            return true;
        }
    }
    else if (e_ident[EI_CLASS] == ELFCLASS32)
    {
        Elf32_Ehdr *header = (Elf32_Ehdr *)(*file_map);
        if (header->e_type != ET_EXEC && header->e_type != ET_DYN && header->e_type != ET_REL)
        {
            ft_printf("ft_nm :%s: File format not recognized\n", file);
            close(fd);
			munmap(*file_map, page_size);
            return true;
        }
    }
    else
    {
        ft_printf("ft_nm :%s: File format not recognized\n", file);
        close(fd);
        return true;
    }
	// if the file is larger than the page size, unmap it
	if (file_stat.st_size > page_size)
	{
		munmap(*file_map, page_size);
		*file_map = NULL;
	}
    return false;
}


bool	get_file_map(void **file_map, size_t file_size, int fd)
{
	// if file_map is not already mapped
	// map it (can be already mapped if the file is smaller than the page size)
	if (!(*file_map))
	{
		*file_map = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (*file_map == MAP_FAILED)
		{
			perror("Error mapping file");
			close(fd);
			return true;
		}
	}
	return false;
}


void	get_32_64_header_and_section(void *file_map, Elf64_Ehdr **header64, Elf32_Ehdr **header32, Elf64_Shdr **sections_header64, Elf32_Shdr **sections_header32)
{
	unsigned char *e_ident = (unsigned char *)file_map;
	if (e_ident[EI_CLASS] == ELFCLASS64)
	{
		*header64 = (Elf64_Ehdr *)file_map;
		*sections_header64 = (Elf64_Shdr *)(file_map + (*header64)->e_shoff);
	}
	else if (e_ident[EI_CLASS] == ELFCLASS32)
	{
		*header32 = (Elf32_Ehdr *)file_map;
		*sections_header32 = (Elf32_Shdr *)(file_map + (*header32)->e_shoff);
	}
}


static int compare_symbols(const char *symA, const char *symB) {
    size_t i = 0, j = 0;

    while (symA[i] != '\0' && symB[j] != '\0') 
	{
        while (symA[i] != '\0' && !ft_isalnum((unsigned char)symA[i]))
            i++;
        while (symB[j] != '\0' && !ft_isalnum((unsigned char)symB[j]))
            j++;

        // if one of the strings is finished
        if (symA[i] == '\0' || symB[j] == '\0')
            break;

        char normalizedA = ft_tolower((unsigned char)symA[i]);
        char normalizedB = ft_tolower((unsigned char)symB[j]);
        if (normalizedA != normalizedB)
            return normalizedA - normalizedB;

        i++;
        j++;
    }

    // cleans up the strings ( exemple : "start" and "start%%%" will have the same normalized string)
    while (symA[i] != '\0' && !ft_isalnum((unsigned char)symA[i]))
        i++;
    while (symB[j] != '\0' && !ft_isalnum((unsigned char)symB[j]))
        j++;

	// if both strings are empty
	if (symA[i] == '\0' && symB[j] == '\0')
        return ft_strcmp(symA, symB);

    return (unsigned char)symA[i] - (unsigned char)symB[j];
}



void sort_symbols_by_name(t_symbol_entry *symbols, size_t symbol_count)
{
    if (!symbols || symbol_count <= 1)
        return;


    for (size_t i = 0; i < symbol_count - 1; i++)
    {
        for (size_t j = 0; j < symbol_count - i - 1; j++)
        {
            if (compare_symbols(symbols[j].name, symbols[j + 1].name) > 0)
            {
                // Échange
                t_symbol_entry tmp = symbols[j];
                symbols[j] = symbols[j + 1];
                symbols[j + 1] = tmp;
            }
        }
    }
}


void process_symbols(t_symbol_entry *symbols, size_t symbol_count)
{
	// sort the symbols by name alphabetically
	sort_symbols_by_name(symbols, symbol_count);
	for (size_t i = 0; i < symbol_count; i++)
	{
		if (symbols[i].type_char == ' ')
			continue;
		if (symbols[i].value == 0)
			printf("                ");
		else
			printf("%016lx", symbols[i].value);
		printf(" %c ", symbols[i].type_char);
		printf("%s\n", symbols[i].name);
	}
}


bool	display_files(const char *file)
{
	int				i;
	int				fd;
	struct stat		file_stat;
	void			*file_map;
	Elf64_Ehdr		*header64 = NULL;
	Elf32_Ehdr		*header32 = NULL;
	Elf64_Shdr		*sections_header64 = NULL;
	Elf32_Shdr		*sections_header32 = NULL;
	t_symbol_entry	*symbols = NULL;
	size_t			symbol_count = 0;


	if (get_fd(file, &fd))
		return true; // error
	if (get_file_stat(&file_stat, file, &fd))
		return true; // error
//--------------------------------------------------------------------------------------------------------//
	// going to be refactored
	if (check_header(&file_map, file_stat, file, fd))
		return true; // error
//--------------------------------------------------------------------------------------------------------//
	// if the file is larger than the page size, map the file
	if (get_file_map(&file_map, file_stat.st_size, fd))
		return true; // error
//--------------------------------------------------------------------------------------------------------//
	// going to be refactored
	// define the elf header and sections(32 bits or 64 bits)
	get_32_64_header_and_section(file_map, &header64, &header32, &sections_header64, &sections_header32);
	if (header64)
	{
		if (save_symbols_64(file_map, sections_header64, header64->e_shnum, &symbols, &symbol_count))
			return true; // error
	}
	// else
	// 	list_symbols_32(file_map, sections_header32, header32->e_shnum);
//--------------------------------------------------------------------------------------------------------//
	process_symbols(symbols, symbol_count);

	return false;
}
