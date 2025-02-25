#include "../../includes/ft_nm.h"

bool is_external_32(Elf32_Sym sym)
{
	unsigned char bind = ELF32_ST_BIND(sym.st_info);
	unsigned char type = ELF32_ST_TYPE(sym.st_info);
	uint16_t shndx = sym.st_shndx;

	if (bind == STB_LOCAL)
		return false;

	if (type == STT_FILE || type == STT_SECTION)
		return false;

	if (bind == STB_WEAK)
		return true;

	if (bind == STB_GLOBAL)
		return true;

	if (shndx == SHN_UNDEF || shndx == SHN_COMMON || shndx == SHN_ABS)
		return true;

	return false;
}

bool is_external_64(Elf64_Sym sym)
{
	unsigned char bind = ELF64_ST_BIND(sym.st_info);
	unsigned char type = ELF64_ST_TYPE(sym.st_info);
	uint16_t shndx = sym.st_shndx;

	if (bind == STB_LOCAL)
		return false;

	if (type == STT_FILE || type == STT_SECTION)
		return false;

	if (bind == STB_WEAK)
		return true;

	if (bind == STB_GLOBAL)
		return true;

	if (shndx == SHN_UNDEF || shndx == SHN_COMMON || shndx == SHN_ABS)
		return true;

	return false;
}