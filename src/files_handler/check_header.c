#include "../../includes/ft_nm.h"
#include <stdbool.h>
#include <stddef.h>

static bool check_header_64 (void *file_map, struct stat file_stat,
							 const char *file, int fd)
{
	Elf64_Ehdr *header = (Elf64_Ehdr *)file_map;
	size_t		file_size = (size_t)file_stat.st_size;

	// Check file type (ET_EXEC, ET_DYN, ET_REL are the only valid types)
	if (header->e_type != ET_EXEC && header->e_type != ET_DYN
		&& header->e_type != ET_REL)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);

	// Check endianness
	if (header->e_ident[EI_DATA] != ELFDATA2LSB
		&& header->e_ident[EI_DATA] != ELFDATA2MSB)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);

	// Check version
	if (header->e_ident[EI_VERSION] != EV_CURRENT
		|| header->e_version != EV_CURRENT)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);

	// Check if the start of the section headers is within the file
	if (header->e_shoff >= file_size)
	{
		print_error_message ("ft_nm: ", file, ERR_FILE_TOO_SHORT);
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	}
	size_t sections_size = header->e_shnum * sizeof (Elf64_Shdr);
	// Check if the end of the section headers is within the file
	if (header->e_shoff + sections_size > file_size)
	{
		print_error_message ("ft_nm: ", file, ERR_FILE_TOO_SHORT);
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	}

	// Check if the section header entry size matches Elf64_Shdr
	if (header->e_shentsize != sizeof (Elf64_Shdr))
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);

	// Check if there is at least one section
	if (header->e_shnum == 0)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);

	Elf64_Shdr *section_headers
		= (Elf64_Shdr *)((char *)file_map + header->e_shoff);

	// Check if the section header string table index is within the number of
	// sections
	if (header->e_shstrndx >= header->e_shnum)
	{
		print_error_message ("ft_nm: warning: ", file,
							 ERR_CORRUPT_STRING_TABLE);
		cleanup_and_report (file, fd, file_map, "", page_size);
		return false;
	}
	Elf64_Shdr *shstrtab_header = &section_headers[header->e_shstrndx];

	// Check if the section header string table is of type SHT_STRTAB
	if (shstrtab_header->sh_type != SHT_STRTAB)
	{
		print_error_message ("ft_nm: ", file, ERR_SECTION_NAME_OUT_OF_RANGE);
		print_error_message ("ft_nm: warning: ", file,
							 ERR_CORRUPT_STRING_TABLE);
		cleanup_and_report (file, fd, file_map, "", page_size);
		return false;
	}

	// Check if the section header string table is within the file
	if (shstrtab_header->sh_offset >= file_size
		|| shstrtab_header->sh_size >= file_size
		|| shstrtab_header->sh_offset + shstrtab_header->sh_size > file_size)
	{
		print_error_message ("ft_nm: ", file, ERR_SECTION_NAME_OUT_OF_RANGE);
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	}

	return false;
}
static bool check_header_32 (void *file_map, struct stat file_stat,
							 const char *file, int fd)
{
	Elf32_Ehdr *header = (Elf32_Ehdr *)file_map;
	size_t		file_size = (size_t)file_stat.st_size;

	if (header->e_type != ET_EXEC && header->e_type != ET_DYN
		&& header->e_type != ET_REL)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	if (header->e_ident[EI_DATA] != ELFDATA2LSB
		&& header->e_ident[EI_DATA] != ELFDATA2MSB)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	if (header->e_ident[EI_VERSION] != EV_CURRENT
		|| header->e_version != EV_CURRENT)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	if (header->e_shoff >= file_size)
	{
		print_error_message ("ft_nm: ", file, ERR_FILE_TOO_SHORT);
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	}
	size_t sections_size = header->e_shnum * sizeof (Elf32_Shdr);
	if (header->e_shoff + sections_size > file_size)
	{
		print_error_message ("ft_nm: ", file, ERR_FILE_TOO_SHORT);
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	}
	if (header->e_shentsize != sizeof (Elf32_Shdr))
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	if (header->e_shnum == 0)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);

	if (header->e_shstrndx >= header->e_shnum)
		print_error_message ("ft_nm: warning: ", file,
							 ERR_CORRUPT_STRING_TABLE);
	Elf32_Shdr *section_headers
		= (Elf32_Shdr *)((char *)file_map + header->e_shoff);

	// Check if the section header string table index is within the number of
	// sections
	if (header->e_shstrndx >= header->e_shnum)
	{
		print_error_message ("ft_nm: warning: ", file,
							 ERR_CORRUPT_STRING_TABLE);
		cleanup_and_report (file, fd, file_map, "", page_size);
		return false;
	}
	Elf32_Shdr *shstrtab_header = &section_headers[header->e_shstrndx];

	// Check if the section header string table is of type SHT_STRTAB
	if (shstrtab_header->sh_type != SHT_STRTAB)
	{
		print_error_message ("ft_nm: ", file, ERR_SECTION_NAME_OUT_OF_RANGE);
		print_error_message ("ft_nm: warning: ", file,
							 ERR_CORRUPT_STRING_TABLE);
		cleanup_and_report (file, fd, file_map, "", page_size);
		return false;
	}

	// Check if the section header string table is within the file
	if (shstrtab_header->sh_offset >= file_size
		|| shstrtab_header->sh_size >= file_size
		|| shstrtab_header->sh_offset + shstrtab_header->sh_size > file_size)
	{
		print_error_message ("ft_nm: ", file, ERR_SECTION_NAME_OUT_OF_RANGE);
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	}
	return false;
}

bool check_header (void *file_map, struct stat file_stat, const char *file,
				   int fd)
{
	unsigned char *e_ident = (unsigned char *)file_map;

	// check if the file is an ELF file (we can also EI_MAG0, EI_MAG1, EI_MAG2,
	// EI_MAG3 it's the same as ELFMAG)
	if (ft_memcmp (e_ident, ELFMAG, SELFMAG) != 0)
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	if (e_ident[EI_CLASS] == ELFCLASS64)
	{
		if (check_header_64 (file_map, file_stat, file, fd))
			return true;
		is_64 = true;
	}
	else if (e_ident[EI_CLASS] == ELFCLASS32)
	{
		if (check_header_32 (file_map, file_stat, file, fd))
			return true;
		is_64 = false;
	}
	else
		return cleanup_and_report (file, fd, file_map,
								   ERR_FORMAT_NOT_RECOGNIZED, page_size);
	return false;
}