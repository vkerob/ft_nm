#include "../../includes/ft_nm.h"


size_t	get_count_32(void *file_map, Elf32_Shdr *sections, Elf32_Shdr *target_section, Elf32_Shdr *strtab_section)
{
	Elf32_Sym	*symtab;
	const char	*strtab;
	size_t		symbol_count;

	// get the symbol table section
	symtab = (Elf32_Sym *)((char*)file_map + target_section->sh_offset);
	// get the string table section associated with the target section
	strtab = (const char *)file_map + strtab_section->sh_offset;

	symbol_count = target_section->sh_size / sizeof(Elf32_Sym);

	for (size_t s = 0; s < symbol_count; s++)
	{
		// get the actual symbol
		Elf32_Sym sym = symtab[s];

		// get the name of the symbol
		const char *sym_name = strtab + sym.st_name;

		if (*sym_name == '\0')
			symbol_count--;
	}
	return symbol_count;
}


size_t	get_count_64(void *file_map, Elf64_Shdr *sections, Elf64_Shdr *target_section, Elf64_Shdr *strtab_section)
{
	Elf64_Sym	*symtab;
	const char	*strtab;
	size_t		symbol_count;

	// get the symbol table section
	symtab = (Elf64_Sym *)((char*)file_map + target_section->sh_offset);
	// get the string table section associated with the target section
	strtab = (const char *)file_map + strtab_section->sh_offset;

	symbol_count = target_section->sh_size / sizeof(Elf64_Sym);

	for (size_t s = 0; s < symbol_count; s++)
	{
		// get the actual symbol
		Elf64_Sym sym = symtab[s];

		// get the name of the symbol
		const char *sym_name = strtab + sym.st_name;

		if (*sym_name == '\0')
			symbol_count--;
	}
	return symbol_count;
}


size_t	get_symbols_count_32(void *file_map, Elf32_Shdr *sections, Elf32_Half sections_count)
{
	Elf32_Shdr		*target_section = NULL; // symtab or dynsym
	Elf32_Shdr		*strtab_section = NULL;
	size_t 			symbol_count = 0;


	// keep variables for the symbol table and string table
	for (int i = 0; i < sections_count; i++) 
	{
        if (sections[i].sh_type == SHT_SYMTAB) // || (sections[i].sh_type == SHT_DYNSYM && flag_dynsym))
		{
			target_section = &sections[i];
			strtab_section = &sections[target_section->sh_link]; // sh_link is the index of the string table section
			symbol_count += get_count_32(file_map, sections, target_section, strtab_section);
		}
    }
	return symbol_count;
}


size_t	get_symbols_count_64(void *file_map, Elf64_Shdr *sections, Elf64_Half sections_count)
{
	Elf64_Shdr		*target_section = NULL; // symtab or dynsym
	Elf64_Shdr		*strtab_section = NULL;
	size_t 			symbol_count = 0;


	// keep variables for the symbol table and string table
	for (int i = 0; i < sections_count; i++) 
	{
        if (sections[i].sh_type == SHT_SYMTAB) // || (sections[i].sh_type == SHT_DYNSYM && flag_dynsym))
		{
			target_section = &sections[i];
			strtab_section = &sections[target_section->sh_link]; // sh_link is the index of the string table section
			symbol_count += get_count_64(file_map, sections, target_section, strtab_section);
		}
    }
	return symbol_count;
}