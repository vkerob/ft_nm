#include "../../includes/ft_nm.h"

char get_symbol_type_char_32(Elf32_Sym sym, Elf32_Shdr *sections)
{
    // get the section index of the symbol
    uint16_t shndx = sym.st_shndx;
    // get the type and binding of the symbol
    unsigned char type = ELF32_ST_TYPE(
        sym.st_info); // get the type of the symbol (function, object, etc.)
    unsigned char bind = ELF32_ST_BIND(
        sym.st_info); // get the binding of the symbol (local, global, etc.)

    if (type == STT_FILE && print_debug_syms)
        return (bind == STB_LOCAL) ? 'a' : 'A';
    else if (type == STT_FILE)
        return ' ';
    // Si tu veux conserver le filtrage des symboles de section, tu peux garder
    // cette condition
    if (type == STT_SECTION)
        return ' ';

    // STB_WEAK (w, W, v, V)
    if (bind == STB_WEAK)
    {
        if (type == STT_OBJECT)
            return (shndx == SHN_UNDEF) ? 'v' : 'V';
        else
            return (shndx == SHN_UNDEF) ? 'w' : 'W';
    }

    if (shndx == SHN_UNDEF)
        return 'U';
    if (shndx == SHN_COMMON)
        return 'C';
    if (shndx == SHN_ABS)
        return 'A';

    // IFUNC
    if (type == STT_GNU_IFUNC)
        return 'I';

    // get the section of the symbol
    Elf32_Shdr section = sections[shndx]; // get the section of the symbol
    Elf32_Word flags = section.sh_flags;  // get the flags of the section (read,
                                          // write, execute, etc.)
    Elf32_Word type_section =
        section.sh_type; // get the type of the section (progbits, nobits, etc.)

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
    if ((type_section == SHT_DYNAMIC || type_section == SHT_INIT_ARRAY ||
         type_section == SHT_FINI_ARRAY || type_section == SHT_PREINIT_ARRAY) &&
        (flags & SHF_ALLOC))
    {
        return (bind == STB_LOCAL) ? 'd' : 'D';
    }

    if (type_section == SHT_NOTE)
    {
        if (flags & SHF_ALLOC)
            return (bind == STB_LOCAL) ? 'r' : 'R';
        else
            return (bind == STB_LOCAL) ? 'n' : 'N';
    }

    if (type_section == SHT_PROGBITS)
    {
        // data : ALLOC + WRITE + !EXEC
        if ((flags & SHF_ALLOC) && (flags & SHF_WRITE) &&
            !(flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 'd' : 'D';

        // code : ALLOC + EXEC
        if ((flags & SHF_ALLOC) && (flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 't' : 'T';

        // rodata : ALLOC + !WRITE + !EXEC
        if ((flags & SHF_ALLOC) && !(flags & SHF_WRITE) &&
            !(flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 'r' : 'R';
    }

    return '?';
}

char get_symbol_type_char_64(Elf64_Sym sym, Elf64_Shdr *sections)
{
    // get the section index of the symbol
    uint16_t shndx = sym.st_shndx;
    // get the type and binding of the symbol
    unsigned char type = ELF64_ST_TYPE(
        sym.st_info); // get the type of the symbol (function, object, etc.)
    unsigned char bind = ELF64_ST_BIND(
        sym.st_info); // get the binding of the symbol (local, global, etc.)

    if (type == STT_FILE && print_debug_syms)
        return (bind == STB_LOCAL) ? 'a' : 'A';
    else if (type == STT_FILE)
        return ' ';
    // Si tu veux conserver le filtrage des symboles de section, tu peux garder
    // cette condition
    if (type == STT_SECTION)
        return ' ';

    // STB_WEAK (w, W, v, V)
    if (bind == STB_WEAK)
    {
        if (type == STT_OBJECT)
            return (shndx == SHN_UNDEF) ? 'v' : 'V';
        else
            return (shndx == SHN_UNDEF) ? 'w' : 'W';
    }

    if (shndx == SHN_UNDEF)
        return 'U';
    if (shndx == SHN_COMMON)
        return 'C';
    if (shndx == SHN_ABS)
        return 'A';

    // IFUNC
    if (type == STT_GNU_IFUNC)
        return 'I';

    // get the section of the symbol
    Elf64_Shdr section = sections[shndx]; // get the section of the symbol
    Elf64_Word flags = section.sh_flags;  // get the flags of the section (read,
                                          // write, execute, etc.)
    Elf64_Word type_section =
        section.sh_type; // get the type of the section (progbits, nobits, etc.)
                         // Elf64_Word name = section.sh_name; // get the name
                         // of the section

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
    if ((type_section == SHT_DYNAMIC || type_section == SHT_INIT_ARRAY ||
         type_section == SHT_FINI_ARRAY || type_section == SHT_PREINIT_ARRAY) &&
        (flags & SHF_ALLOC))
    {
        return (bind == STB_LOCAL) ? 'd' : 'D';
    }

    if (type_section == SHT_NOTE)
    {
        if (flags & SHF_ALLOC)
            return (bind == STB_LOCAL) ? 'r' : 'R';
        else
            return (bind == STB_LOCAL) ? 'n' : 'N';
    }

    if (type_section == SHT_PROGBITS)
    {
        // data : ALLOC + WRITE + !EXEC
        if ((flags & SHF_ALLOC) && (flags & SHF_WRITE) &&
            !(flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 'd' : 'D';

        // code : ALLOC + EXEC
        if ((flags & SHF_ALLOC) && (flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 't' : 'T';

        // rodata : ALLOC + !WRITE + !EXEC
        if ((flags & SHF_ALLOC) && !(flags & SHF_WRITE) &&
            !(flags & SHF_EXECINSTR))
            return (bind == STB_LOCAL) ? 'r' : 'R';

        // if (!(flags & SHF_ALLOC) && is_section_name(name, ".debug"))
        // 	return 'N';
        // if (!(flags & SHF_ALLOC) && !(flags & SHF_WRITE) && !(flags &
        // SHF_EXECINSTR) && (is_section_name(name, ".note") ||
        // is_section_name(name, ".comment"))) 	return 'n';
        // // p
        // if ((flags & SHF_ALLOC) && (is_section_name(name, ".eh_frame") ||
        // is_section_name(name, ".gcc_except_table"))) 	return 'p';
    }

    return '?';
}