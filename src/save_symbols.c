#include "../includes/ft_nm.h"

// /* Legal values for ST_BIND subfield of st_info (symbol binding).  */

// #define STB_LOCAL	0		/* Local symbol */
// #define STB_GLOBAL	1		/* Global symbol */
// #define STB_WEAK	2		/* Weak symbol */
// #define	STB_NUM		3		/* Number of defined types.  */
// #define STB_LOOS	10		/* Start of OS-specific */
// #define STB_GNU_UNIQUE	10		/* Unique symbol.  */
// #define STB_HIOS	12		/* End of OS-specific */
// #define STB_LOPROC	13		/* Start of processor-specific */
// #define STB_HIPROC	15		/* End of processor-specific */

// /* Legal values for ST_TYPE subfield of st_info (symbol type).  */

// #define STT_NOTYPE	0		/* Symbol type is unspecified */
// #define STT_OBJECT	1		/* Symbol is a data object */
// #define STT_FUNC	2		/* Symbol is a code object */
// #define STT_SECTION	3		/* Symbol associated with a section */
// #define STT_FILE	4		/* Symbol's name is file name */
// #define STT_COMMON	5		/* Symbol is a common data object */
// #define STT_TLS		6		/* Symbol is thread-local data object*/
// #define	STT_NUM		7		/* Number of defined types.  */
// #define STT_LOOS	10		/* Start of OS-specific */
// #define STT_GNU_IFUNC	10		/* Symbol is indirect code object */
// #define STT_HIOS	12		/* End of OS-specific */
// #define STT_LOPROC	13		/* Start of processor-specific */
// #define STT_HIPROC	15		/* End of processor-specific */


// /* Symbol table indices are found in the hash buckets and chain table
//    of a symbol hash table section.  This special index value indicates
//    the end of a chain, meaning no further symbols are found in that bucket.  */

// #define STN_UNDEF	0		/* End of a chain.  */


bool	is_section_name(Elf64_Word name, const char *section_name)
{
	return (ft_strncmp(section_name, (const char *)(section_name + name), ft_strlen(section_name)) == 0);
}


static char get_symbol_type_char_64(Elf64_Sym sym, Elf64_Shdr *sections)
{
	// get the section index of the symbol
    uint16_t shndx = sym.st_shndx;
	// get the type and binding of the symbol
	unsigned char type = ELF64_ST_TYPE(sym.st_info); // get the type of the symbol (function, object, etc.)
	unsigned char bind = ELF64_ST_BIND(sym.st_info); // get the binding of the symbol (local, global, etc.)


	if (type == STT_FILE || type == STT_SECTION)
		return ' ';
	

	// STB_WEAK (w, W, v, V)
    if (bind == STB_WEAK)
    {
        if (type == STT_OBJECT)
            return (shndx == SHN_UNDEF) ? 'v' : 'V';
        else
            return (shndx == SHN_UNDEF) ? 'w' : 'W';
    }

	if (shndx == SHN_UNDEF)  return 'U';
    if (shndx == SHN_COMMON) return 'C';
    if (shndx == SHN_ABS)    return 'A';

	// IFUNC
	if (type == STT_GNU_IFUNC)
		return 'I';

	// get the section of the symbol
	Elf64_Shdr section = sections[shndx]; // get the section of the symbol
	Elf64_Word flags = section.sh_flags; // get the flags of the section (read, write, execute, etc.)
	Elf64_Word type_section = section.sh_type; // get the type of the section (progbits, nobits, etc.)
	// Elf64_Word name = section.sh_name; // get the name of the section

	// SHT_NOBITS: section that occupies no space in the file
	// SHT_PROGBITS: section that occupies space in the file

	// SHF_ALLOC: section is allocated in memory
	// SHF_WRITE: section is writable
	// SHF_EXECINSTR: section contains executable instructions

	// b, B: uninitialized data section (readable, writable)
	// d, D: initialized data section (readable, writable)
	// r, R: read-only data section

	// b/B => BSS : SHT_NOBITS + SHF_ALLOC
    if (type_section == SHT_NOBITS && (flags & SHF_ALLOC))
        return (bind == STB_LOCAL) ? 'b' : 'B';

    // d/D => SHT_DYNAMIC, SHT_INIT_ARRAY, SHT_FINI_ARRAY, etc.
    if ((type_section == SHT_DYNAMIC
      || type_section == SHT_INIT_ARRAY
      || type_section == SHT_FINI_ARRAY
      || type_section == SHT_PREINIT_ARRAY)
      && (flags & SHF_ALLOC))
    {
        return (bind == STB_LOCAL) ? 'd' : 'D';
    }

    // NOTE => si ALLOC => r/R, sinon => n/N (nm considère .note alloué comme rodata)
    if (type_section == SHT_NOTE)
    {
        if (flags & SHF_ALLOC)
            return (bind == STB_LOCAL) ? 'r' : 'R';
        else
            return (bind == STB_LOCAL) ? 'n' : 'N';
    }

    /* ----------------------------------
       3) SHT_PROGBITS : data, code, rodata
       ---------------------------------- */
    if (type_section == SHT_PROGBITS)
    {
        // data : ALLOC + WRITE + !EXEC
        if ((flags & SHF_ALLOC) && (flags & SHF_WRITE) && !(flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 'd' : 'D';

        // code : ALLOC + EXEC
        if ((flags & SHF_ALLOC) && (flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 't' : 'T';

        // rodata : ALLOC + !WRITE + !EXEC
        if ((flags & SHF_ALLOC) && !(flags & SHF_WRITE) && !(flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 'r' : 'R';

		// if (!(flags & SHF_ALLOC) && is_section_name(name, ".debug"))
		// 	return 'N';
		// if (!(flags & SHF_ALLOC) && !(flags & SHF_WRITE) && !(flags & SHF_EXECINSTR) && (is_section_name(name, ".note") || is_section_name(name, ".comment")))
		// 	return 'n';
		// // p
		// if ((flags & SHF_ALLOC) && (is_section_name(name, ".eh_frame") || is_section_name(name, ".gcc_except_table")))
		// 	return 'p';

	}

	return '?';

}


void	set_target_symbols_64(void *file_map, Elf64_Shdr *sections, Elf64_Shdr *target_section, Elf64_Shdr *strtab_section, t_symbol_entry *symbols, size_t *pos_symbols)
{
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

		// get the type of the symbol

		if (*sym_name == '\0')
			continue;

		char type_char = get_symbol_type_char_64(sym, sections);

		symbols[*pos_symbols].name = sym_name;
		symbols[*pos_symbols].value = sym.st_value;
		symbols[*pos_symbols].type_char = type_char;
		(*pos_symbols)++;

	}
}



void set_symbols_64(void *file_map, Elf64_Shdr *sections, t_symbol_entry *symbols, Elf64_Half sections_count)
{
	Elf64_Shdr		*target_section = NULL; // symtab or dynsym
	Elf64_Shdr		*strtab_section = NULL;
	size_t			pos_symbols = 0;

	for (int i = 0; i < sections_count; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)  // || (sections[i].sh_type == SHT_DYNSYM && flag_dynsym))
		{
			target_section = &sections[i];
			strtab_section = &sections[target_section->sh_link];
			set_target_symbols_64(file_map, sections, target_section, strtab_section, symbols, &pos_symbols);
		}
	}

}

size_t	get_count(void *file_map, Elf64_Shdr *sections, Elf64_Shdr *target_section, Elf64_Shdr *strtab_section)
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
			symbol_count += get_count(file_map, sections, target_section, strtab_section);
		}
    }
	return symbol_count;
}


bool	save_symbols_64(void *file_map, Elf64_Shdr *sections, Elf64_Half sections_count, t_symbol_entry **symbols, size_t *symbol_count)
{


	// browse all sections headers to find the symbol table (symtab or dynsym) and count the symbols


	*symbol_count = get_symbols_count_64(file_map, sections, sections_count);
	*symbols = (t_symbol_entry *)malloc(sizeof(t_symbol_entry) * *symbol_count);
	if (!(*symbols))
	{
		perror("malloc");
		return true;
	}
	// browse all sections headers to find the symbol table (symtab or dynsym) and list the symbols
	set_symbols_64(file_map, sections, *symbols, sections_count);

	return false;
}


