#include "../../includes/ft_nm.h"

void set_target_symbols_32(void *file_map, Elf32_Shdr *sections,
                           Elf32_Shdr *target_section,
                           Elf32_Shdr *strtab_section, t_symbol_entry *symbols,
                           size_t *pos_symbols)
{
    // keep variables for the symbol table and string table
    Elf32_Sym *symtab =
        (Elf32_Sym *)((char *)file_map + target_section->sh_offset);
    const char *strtab = (const char *)file_map + strtab_section->sh_offset;
    size_t symbol_count = target_section->sh_size / sizeof(Elf32_Sym);

    // browse all symbols (s = symbol)
    for (size_t s = 0; s < symbol_count; s++)
    {
        // get the actual symbol
        Elf32_Sym sym = symtab[s];

        // get the name of the symbol
        const char *sym_name = strtab + sym.st_name;

        // get the type of the symbol
        char type_char = get_symbol_type_char_32(sym, sections);

        if (is_unvalid_symbol_32(sym, s, symtab, strtab, symbol_count,
                                 sections))
            continue;

        symbols[*pos_symbols].name = sym_name;
        symbols[*pos_symbols].value = (void *)(uintptr_t)sym.st_value;
        symbols[*pos_symbols].type_char = type_char;
        (*pos_symbols)++;
    }
}

void set_target_symbols_64(void *file_map, Elf64_Shdr *sections,
                           Elf64_Shdr *target_section,
                           Elf64_Shdr *strtab_section, t_symbol_entry *symbols,
                           size_t *pos_symbols)
{
    // keep variables for the symbol table and string table
    Elf64_Sym *symtab =
        (Elf64_Sym *)((char *)file_map + target_section->sh_offset);
    const char *strtab = (const char *)file_map + strtab_section->sh_offset;
    size_t symbol_count = target_section->sh_size / sizeof(Elf64_Sym);

    // browse all symbols (s = symbol)
    for (size_t s = 0; s < symbol_count; s++)
    {
        // get the actual symbol
        Elf64_Sym sym = symtab[s];

        // get the name of the symbol
        const char *sym_name = strtab + sym.st_name;

        // get the type of the symbol
        char type_char = get_symbol_type_char_64(sym, sections);

        if (is_unvalid_symbol_64(sym, s, symtab, strtab, symbol_count,
                                 sections))
            continue;

        symbols[*pos_symbols].name = sym_name;
        symbols[*pos_symbols].value = (void *)(uintptr_t)sym.st_value;
        symbols[*pos_symbols].type_char = type_char;
        (*pos_symbols)++;
    }
}

void set_symbols_32(void *file_map, Elf32_Shdr *sections,
                    t_symbol_entry *symbols, Elf32_Half sections_count)
{
    Elf32_Shdr *target_section = NULL; // symtab or dynsym
    Elf32_Shdr *strtab_section = NULL;
    size_t pos_symbols = 0;

    for (int i = 0; i < sections_count; i++)
    {
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            target_section = &sections[i];
            strtab_section = &sections[target_section->sh_link];
            set_target_symbols_32(file_map, sections, target_section,
                                  strtab_section, symbols, &pos_symbols);
        }
    }
}

void set_symbols_64(void *file_map, Elf64_Shdr *sections,
                    t_symbol_entry *symbols, Elf64_Half sections_count)
{
    Elf64_Shdr *target_section = NULL; // symtab or dynsym
    Elf64_Shdr *strtab_section = NULL;
    size_t pos_symbols = 0;

    for (int i = 0; i < sections_count; i++)
    {
        if (sections[i].sh_type == SHT_SYMTAB)
        {
            target_section = &sections[i];
            strtab_section = &sections[target_section->sh_link];
            set_target_symbols_64(file_map, sections, target_section,
                                  strtab_section, symbols, &pos_symbols);
        }
    }
}