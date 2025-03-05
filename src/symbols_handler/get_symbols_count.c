#include "../../includes/ft_nm.h"


size_t get_count_64(void *file_map, Elf64_Shdr *sections,
					Elf64_Shdr *target_section, Elf64_Shdr *strtab_section,
					size_t file_size)
{
	Elf64_Sym *symtab;
	const char *strtab;
	size_t total_symbols;
	size_t valid_count = 0;
	
	// check if the section index is not out of bounds
	if (target_section->sh_offset + target_section->sh_size > file_size ||
		strtab_section->sh_offset + strtab_section->sh_size > file_size)
		return -1;

	// get the symbol table section
	symtab = (Elf64_Sym *)((char *)file_map + target_section->sh_offset);
	// get the string table section associated with the target section
	strtab = (const char *)file_map + strtab_section->sh_offset;
	total_symbols = target_section->sh_size / sizeof(Elf64_Sym);
	
	for (size_t s = 0; s < total_symbols; s++)
	{
		// get the actual symbol
		Elf64_Sym sym = symtab[s];
		
		if (is_unvalid_symbol_64(sym, strtab, sections))
		continue;
		
		valid_count++;
	}
	return valid_count;
}

size_t get_count_32(void *file_map, Elf32_Shdr *sections,
					Elf32_Shdr *target_section, Elf32_Shdr *strtab_section,
					size_t file_size)
{
	Elf32_Sym *symtab;
	const char *strtab;
	size_t total_symbols;
	size_t valid_count = 0;

	if (target_section->sh_offset + target_section->sh_size > file_size ||
		strtab_section->sh_offset + strtab_section->sh_size > file_size)
		return -1;

	symtab = (Elf32_Sym *)((char *)file_map + target_section->sh_offset);
	strtab = (const char *)file_map + strtab_section->sh_offset;
	total_symbols = target_section->sh_size / sizeof(Elf32_Sym);
	valid_count = 0;

	for (size_t s = 0; s < total_symbols; s++)
	{
		Elf32_Sym sym = symtab[s];

		if (is_unvalid_symbol_32(sym, strtab, sections))
			continue;

		valid_count++;
	}
	return valid_count;
}
	
long get_symbols_count_64(void *file_map, Elf64_Shdr *sections,
							Elf64_Half sections_count, size_t file_size)
{
	Elf64_Shdr *target_section = NULL; // symtab
	Elf64_Shdr *strtab_section = NULL;
	long symbols_count = 0;
	long symbol_count = 0;
	
	for (int i = 0; i < sections_count; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
		// check if the section index is not out of bounds
			if (sections[i].sh_link >= sections_count)
				return -1;

			target_section = &sections[i];
			strtab_section =
				&sections[target_section->sh_link]; // sh_link is the index of
													// the string table section
			symbol_count = get_count_64(file_map, sections, target_section,
										strtab_section, file_size);
			if (symbol_count == -1)
				return -1;
			symbols_count += symbol_count;
		}
	}
	return symbols_count;
}

long get_symbols_count_32(void *file_map, Elf32_Shdr *sections,
							Elf32_Half sections_count, size_t file_size)
{
	Elf32_Shdr *target_section = NULL;
	Elf32_Shdr *strtab_section = NULL;
	long symbols_count = 0;
	long symbol_count = 0;

	for (int i = 0; i < sections_count; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			if (sections[i].sh_link >= sections_count)
				return -1;
			target_section = &sections[i];
			strtab_section =
				&sections[target_section->sh_link];
			symbol_count = get_count_32(file_map, sections, target_section,
										strtab_section, file_size);
			if (symbol_count == -1)
				return -1;
			symbols_count += symbol_count;
		}
	}
	return symbol_count;
}