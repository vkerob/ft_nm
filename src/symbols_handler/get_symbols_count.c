#include "../../includes/ft_nm.h"
#include <elf.h>

size_t get_count_64 (Elf64_Ehdr *header, Elf64_Shdr *sections,
					 Elf64_Shdr *symtab_section, Elf64_Shdr *strtab_section,
					 size_t file_size)
{
	// Check if the symbol table and string table sections are within file
	// boundaries
	if (symtab_section->sh_offset + symtab_section->sh_size > file_size
		|| strtab_section->sh_offset + strtab_section->sh_size > file_size)
		return 0;

	// Retrieve the start of the symbol table and its associated string table
	Elf64_Sym *symtab
		= (Elf64_Sym *)((char *)header + symtab_section->sh_offset);
	const char *strtab = (const char *)header + strtab_section->sh_offset;

	size_t total_symbols = symtab_section->sh_size / sizeof (Elf64_Sym);
	size_t valid_count = 0;

	// Iterate over all symbols in the symbol table
	for (size_t s = 0; s < total_symbols; s++)
	{
		// Get the current symbol
		Elf64_Sym sym = symtab[s];

		// Skip the symbol if it is not valid
		if (is_unvalid_symbol_64 (sym, strtab, sections))
			continue;

		valid_count++;
	}

	return valid_count;
}

size_t get_count_32 (Elf32_Ehdr *header, Elf32_Shdr *sections,
					 Elf32_Shdr *symtab_section, Elf32_Shdr *strtab_section,
					 size_t file_size)
{
	if (symtab_section->sh_offset + symtab_section->sh_size > file_size
		|| strtab_section->sh_offset + strtab_section->sh_size > file_size)
		return 0;

	Elf32_Sym *symtab
		= (Elf32_Sym *)((char *)header + symtab_section->sh_offset);
	const char *strtab = (const char *)header + strtab_section->sh_offset;
	size_t		total_symbols = symtab_section->sh_size / sizeof (Elf32_Sym);
	size_t		valid_count = 0;

	for (size_t s = 0; s < total_symbols; s++)
	{
		Elf32_Sym sym = symtab[s];

		if (is_unvalid_symbol_32 (sym, strtab, sections))
			continue;

		valid_count++;
	}
	return valid_count;
}

// browse all sections headers to find symtab and count the symbols
// to find symtab section we need the name of the section and the type of the
// section if we find the symtab section we get the strtab section associated
size_t get_symbols_count_64 (Elf64_Ehdr *header, Elf64_Shdr *section_headers,
							 Elf64_Shdr **symtab_section,
							 Elf64_Shdr **strtab_section, size_t file_size)
{
	// Section header string table, symbol table section and its associated
	// string table
	Elf64_Shdr *shstrtab = NULL;	  // Section header string table
	const char *section_names = NULL; // Pointer to the section names
	size_t		symbol_count = 0;

	// Check if the section header string table index is within bounds
	if (header->e_shstrndx >= header->e_shnum)
		return 0;

	// Retrieve the section header string table and verify its offset is valid
	shstrtab = &section_headers[header->e_shstrndx];
	if (shstrtab->sh_offset >= file_size)
		return 0;
	section_names = (const char *)header + shstrtab->sh_offset;

	// Browse all section headers to find the symbol table (".symtab") and count
	// its symbols
	for (Elf64_Half i = 0; i < header->e_shnum; i++)
	{
		// Ensure the section name offset is within the file boundaries
		if (shstrtab->sh_offset + section_headers[i].sh_name >= file_size)
			break;

		// Get the section name (section_headers[i].sh_name is an offset into
		// shstrtab)
		const char *secname = section_names + section_headers[i].sh_name;
		if (!secname)
			break; // Safety check (should not occur if section_names is valid)

		// Check if the section is a symbol table by type and name
		if (section_headers[i].sh_type == SHT_SYMTAB
			&& ft_strcmp (secname, ".symtab") == 0)
		{
			// Ensure the linked index (associated string table) is within
			// bounds
			if (section_headers[i].sh_link >= header->e_shnum)
				break;

			*symtab_section = &section_headers[i];
			*strtab_section = &section_headers[(*symtab_section)->sh_link];

			// Get the symbol count from the symbol table using a helper
			// function
			symbol_count
				= get_count_64 (header, section_headers, *symtab_section,
								*strtab_section, file_size);
			if (symbol_count == 0)
				break;
			return symbol_count;
		}
	}

	return 0;
}

size_t get_symbols_count_32 (Elf32_Ehdr *header, Elf32_Shdr *section_headers,
							 Elf32_Shdr **symtab_section,
							 Elf32_Shdr **strtab_section, size_t file_size)
{
	Elf32_Shdr *shstrtab = NULL;
	const char *section_names = NULL;
	size_t		symbol_count = 0;

	if (header->e_shstrndx >= header->e_shnum)
		return 0;
	shstrtab = &section_headers[header->e_shstrndx];
	if (shstrtab->sh_offset >= file_size)
		return 0;
	section_names = (const char *)header + shstrtab->sh_offset;

	for (Elf32_Half i = 0; i < header->e_shnum; i++)
	{
		if (shstrtab->sh_offset + section_headers[i].sh_name >= file_size)
			break;
		const char *secname = section_names + section_headers[i].sh_name;
		if (!secname)
			break;
		if (section_headers[i].sh_type == SHT_SYMTAB
			&& ft_strcmp (secname, ".symtab") == 0)
		{
			if (section_headers[i].sh_link >= header->e_shnum)
				break;
			*symtab_section = &section_headers[i];
			*strtab_section = &section_headers[(*symtab_section)->sh_link];
			symbol_count
				= get_count_32 (header, section_headers, *symtab_section,
								*strtab_section, file_size);
			if (symbol_count == 0)
				break;
			return symbol_count;
		}
	}
	return 0;
}