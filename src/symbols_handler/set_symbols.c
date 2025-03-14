#include "../../includes/ft_nm.h"

void set_symbols_32 (Elf32_Ehdr *header, Elf32_Shdr *section_headers,
					 t_symbol_entry *symbols, Elf32_Shdr *symtab_section,
					 Elf32_Shdr *strtab_section)
{
	// keep variables for the symbol table and string table
	Elf32_Sym *symtab
		= (Elf32_Sym *)((char *)header + symtab_section->sh_offset);
	const char *strtab = (const char *)header + strtab_section->sh_offset;
	size_t		symbol_count = symtab_section->sh_size / sizeof (Elf32_Sym);
	size_t		pos_symbols = 0;

	// browse all symbols (s = symbol)
	for (size_t s = 0; s < symbol_count; s++)
	{
		// get the actual symbol
		Elf32_Sym sym = symtab[s];

		// get the name of the symbol
		const char *sym_name = strtab + sym.st_name;

		// get the type of the symbol
		char type_char = get_symbol_type_char_32 (sym, section_headers);

		if (is_unvalid_symbol_32 (sym, strtab, section_headers))
			continue;

		symbols[pos_symbols].name = sym_name;
		symbols[pos_symbols].value = (void *)(uintptr_t)sym.st_value;
		symbols[pos_symbols].type_char = type_char;
		pos_symbols++;
	}
}

void set_symbols_64 (Elf64_Ehdr *header, Elf64_Shdr *section_headers,
					 t_symbol_entry *symbols, Elf64_Shdr *symtab_section,
					 Elf64_Shdr *strtab_section)
{
	// keep variables for the symbol table and string table
	Elf64_Sym *symtab
		= (Elf64_Sym *)((char *)header + symtab_section->sh_offset);
	const char *strtab = (const char *)header + strtab_section->sh_offset;
	size_t		symbol_count = symtab_section->sh_size / sizeof (Elf64_Sym);
	size_t		pos_symbols = 0;

	// browse all symbols (s = symbol)
	for (size_t s = 0; s < symbol_count; s++)
	{
		// get the actual symbol
		Elf64_Sym sym = symtab[s];

		// get the name of the symbol
		const char *sym_name = strtab + sym.st_name;

		// get the type of the symbol
		char type_char = get_symbol_type_char_64 (sym, section_headers);

		if (is_unvalid_symbol_64 (sym, strtab, section_headers))
			continue;

		symbols[pos_symbols].name = sym_name;
		symbols[pos_symbols].value = (void *)(uintptr_t)sym.st_value;
		symbols[pos_symbols].type_char = type_char;
		pos_symbols++;
	}
}