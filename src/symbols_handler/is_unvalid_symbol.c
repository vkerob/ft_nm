#include "../../includes/ft_nm.h"

bool is_unvalid_symbol_64 (Elf64_Sym sym, const char *strtab,
						   Elf64_Shdr *sections)
{
	const char *sym_name = strtab + sym.st_name;
	char		type_char = get_symbol_type_char_64 (sym, sections);

	if (*sym_name == '\0' && type_char != 'a')
		return true;

	if (undefined_only && sym.st_shndx != SHN_UNDEF)
		return true;

	if (external_only && !is_external_64 (sym))
		return true;

	return false;
}

bool is_unvalid_symbol_32 (Elf32_Sym sym, const char *strtab,
						   Elf32_Shdr *sections)
{
	const char *sym_name = strtab + sym.st_name;
	char		type_char = get_symbol_type_char_32 (sym, sections);

	if (*sym_name == '\0' && type_char != 'a')
		return true;

	if (undefined_only && sym.st_shndx != SHN_UNDEF)
		return true;

	if (external_only && !is_external_32 (sym))
		return true;

	return false;
}
