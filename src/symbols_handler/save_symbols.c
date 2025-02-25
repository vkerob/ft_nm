#include "../../includes/ft_nm.h"

static void get_64_header_and_section(void *file_map, Elf64_Ehdr **header64,
									  Elf64_Shdr **sections_header64)
{
	*header64 = (Elf64_Ehdr *)file_map;
	*sections_header64 = (Elf64_Shdr *)(file_map + (*header64)->e_shoff);
}

static void get_32_header_and_section(void *file_map, Elf32_Ehdr **header32,
									  Elf32_Shdr **sections_header32)
{
	*header32 = (Elf32_Ehdr *)file_map;
	*sections_header32 = (Elf32_Shdr *)(file_map + (*header32)->e_shoff);
}

bool save_symbols_64(void *file_map, t_symbol_entry **symbols,
					 size_t *symbol_count, const char *file_name)
{

	Elf64_Ehdr *header;
	Elf64_Shdr *sections_header;
	size_t sections_count;

	// browse all sections headers to find the symbol table (symtab or dynsym)
	// and count the symbols

	get_64_header_and_section(file_map, &header, &sections_header);

	sections_count = header->e_shnum;

	*symbol_count =
		get_symbols_count_64(file_map, sections_header, sections_count);
	if (*symbol_count == 0)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd((char *)file_name, 2);
		ft_putstr_fd(": no symbols\n", 2);
		return true;
	}
	*symbols = (t_symbol_entry *)malloc(sizeof(t_symbol_entry) * *symbol_count);
	if (!(*symbols))
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd((char *)file_name, 2);
		ft_putstr_fd(": malloc failed\n", 2);
		return true;
	}
	// browse all sections headers to find the symbol table (symtab or dynsym)
	// and list the symbols
	set_symbols_64(file_map, sections_header, *symbols, sections_count);

	return false;
}

bool save_symbols_32(void *file_map, t_symbol_entry **symbols,
					 size_t *symbol_count, const char *file_name)
{
	Elf32_Ehdr *header;
	Elf32_Shdr *sections_header;
	size_t sections_count;

	// browse all sections headers to find the symbol table (symtab or dynsym)
	// and count the symbols
	get_32_header_and_section(file_map, &header, &sections_header);

	sections_count = header->e_shnum;

	*symbol_count =
		get_symbols_count_32(file_map, sections_header, sections_count);
	if (*symbol_count == 0)
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd((char *)file_name, 2);
		ft_putstr_fd(": no symbols\n", 2);
		return true;
	}
	*symbols = (t_symbol_entry *)malloc(sizeof(t_symbol_entry) * *symbol_count);
	if (!(*symbols))
	{
		ft_putstr_fd("ft_nm: ", 2);
		ft_putstr_fd((char *)file_name, 2);
		ft_putstr_fd(": malloc failed\n", 2);
		return true;
	}
	// browse all sections headers to find the symbol table (symtab or dynsym)
	// and list the symbols
	set_symbols_32(file_map, sections_header, *symbols, sections_count);

	return false;
}

bool save_symbols(void *file_map, t_symbol_entry **symbols,
				  size_t *symbol_count, const char *file_name)
{
	if (is_64)
	{
		if (save_symbols_64(file_map, symbols, symbol_count, file_name))
			return true; // error
	}
	else
	{
		if (save_symbols_32(file_map, symbols, symbol_count, file_name))
			return true; // error
	}
	return false;
}