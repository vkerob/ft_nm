#include "../../includes/ft_nm.h"

static void get_64_header_and_section_headers (void		   *file_map,
											   Elf64_Ehdr **header64,
											   Elf64_Shdr **section_headers64)
{
	*header64 = (Elf64_Ehdr *)file_map;
	*section_headers64 = (Elf64_Shdr *)(file_map + (*header64)->e_shoff);
}

static void get_32_header_and_section_headers (void		   *file_map,
											   Elf32_Ehdr **header32,
											   Elf32_Shdr **section_headers32)
{
	*header32 = (Elf32_Ehdr *)file_map;
	*section_headers32 = (Elf32_Shdr *)(file_map + (*header32)->e_shoff);
}

// browse all sections headers to find the symbol table (symtab or dynsym)
// and count the symbols
bool save_symbols_64 (Elf64_Ehdr *header, Elf64_Shdr *section_headers,
					  t_symbol_entry **symbols, long *symbol_count,
					  const char *file_name, size_t file_size)
{

	Elf64_Shdr *symtab_section = NULL;
	Elf64_Shdr *strtab_section = NULL;

	*symbol_count = get_symbols_count_64 (
		header, section_headers, &symtab_section, &strtab_section, file_size);
	if (*symbol_count == 0)
	{
		print_error_message ("ft_nm: ", file_name, ERR_NO_SYMBOLS);
		return true;
	}

	*symbols
		= (t_symbol_entry *)malloc (sizeof (t_symbol_entry) * *symbol_count);
	if (!(*symbols))
	{
		print_error_message ("ft_nm: ", file_name, ERR_MALLOC_FAILED);
		return true;
	}
	set_symbols_64 (header, section_headers, *symbols, symtab_section,
					strtab_section);

	return false;
}

bool save_symbols_32 (Elf32_Ehdr *header, Elf32_Shdr *section_headers,
					  t_symbol_entry **symbols, long *symbol_count,
					  const char *file_name, size_t file_size)
{
	Elf32_Shdr *symtab_section = NULL;
	Elf32_Shdr *strtab_section = NULL;

	*symbol_count = get_symbols_count_32 (
		header, section_headers, &symtab_section, &strtab_section, file_size);
	if (*symbol_count == 0)
	{
		print_error_message ("ft_nm: ", file_name, ERR_NO_SYMBOLS);
		return true;
	}
	*symbols
		= (t_symbol_entry *)malloc (sizeof (t_symbol_entry) * *symbol_count);
	if (!(*symbols))
	{
		print_error_message ("ft_nm: ", file_name, ERR_MALLOC_FAILED);
		return true;
	}
	set_symbols_32 (header, section_headers, *symbols, symtab_section,
					strtab_section);

	return false;
}

bool save_symbols (void *file_map, t_symbol_entry **symbols, long *symbol_count,
				   const char *file_name, size_t file_size)
{
	if (is_64)
	{
		Elf64_Ehdr *header;
		Elf64_Shdr *section_headers;

		get_64_header_and_section_headers (file_map, &header, &section_headers);
		if (save_symbols_64 (header, section_headers, symbols, symbol_count,
							 file_name, file_size))
			return true; // error
	}
	else
	{
		Elf32_Ehdr *header;
		Elf32_Shdr *section_headers;

		get_32_header_and_section_headers (file_map, &header, &section_headers);
		if (save_symbols_32 (header, section_headers, symbols, symbol_count,
							 file_name, file_size))
			return true; // error
	}
	return false;
}