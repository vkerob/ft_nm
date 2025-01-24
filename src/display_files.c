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


// static char	get_symbol_type_64(Elf64_Sym sym, Elf64_Shdr *sections, const char *sym_name, unsigned char bind, unsigned char type)
// {

// }


void list_target_symbols_64(void *file_map, Elf64_Shdr *sections, Elf64_Shdr *target_section, t_symbol_entry *symbols, size_t *pos_symbols)
{
	Elf64_Shdr *strtab_section = NULL;

	// get the string table section associated with the target section
	strtab_section = &sections[target_section->sh_link]; // sh_link is the index of the string table section

	// keep variables for the symbol table and string table
	Elf64_Sym *symtab = (Elf64_Sym *)((char*)file_map + target_section->sh_offset);
	const char *strtab = (const char *)file_map + strtab_section->sh_offset;
	size_t symbol_count = target_section->sh_size / sizeof(Elf64_Sym);

	// browse all symbols (s = symbol)
	for (size_t s = 0; s < symbol_count; s++) {
		// get the actual symbol
		Elf64_Sym sym = symtab[s];
		
		// get the name of the symbol
		const char *sym_name = strtab + sym.st_name;

		unsigned char bind = ELF64_ST_BIND(sym.st_info);
		unsigned char type = ELF64_ST_TYPE(sym.st_info);
		// get the type of the symbol
		// char type_char = get_symbol_type_64(sym, sections, sym_name, bind, type);

		if (*sym_name == '\0')
			continue;

		symbols[*pos_symbols].name = sym_name;
		symbols[*pos_symbols].value = sym.st_value;
		symbols[*pos_symbols].type_char = 'w';
		(*pos_symbols)++;

		// sort the symbols by name alphabetically
	}
}


void list_symbols_64(void *file_map, Elf64_Shdr *sections, Elf64_Half sections_count)
{
    Elf64_Shdr *target_section = NULL; // symtab or dynsym
	size_t symbol_count = 0; // number of symbols in the symtab and dynsym
	t_symbol_entry *symbols = NULL;
	size_t pos_symbols = 0;


	// browse all sections headers to find the symbol table (symtab or dynsym)
    for (int i = 0; i < sections_count; i++) 
	{
        if (sections[i].sh_type == SHT_SYMTAB || sections[i].sh_type == SHT_DYNSYM) 
			symbol_count += sections[i].sh_size / sizeof(Elf64_Sym);
    }
	symbols = (t_symbol_entry *)malloc(sizeof(t_symbol_entry) * symbol_count);
	if (!symbols)
	{
		perror("malloc");
		return;
	}
	for (int i = 0; i < sections_count; i++) 
	{
		if (sections[i].sh_type == SHT_SYMTAB || sections[i].sh_type == SHT_DYNSYM) 
		{
			target_section = &sections[i];
			list_target_symbols_64(file_map, sections, target_section, symbols, &pos_symbols);
		}
	}
}


bool	display_files(const char *file)
{
	int			i;
	int			fd;
	struct stat	file_stat;
	void		*file_map;
	Elf64_Ehdr	*header64 = NULL;
	Elf32_Ehdr	*header32 = NULL;
	Elf64_Shdr	*sections_header64 = NULL;
	Elf32_Shdr	*sections_header32 = NULL;

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
		list_symbols_64(file_map, sections_header64, header64->e_shnum);
	// else
	// 	list_symbols_32(file_map, sections_header32, header32->e_shnum);
//--------------------------------------------------------------------------------------------------------//

	return false;
}
