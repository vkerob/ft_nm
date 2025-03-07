#include "../../includes/ft_nm.h"

static bool check_header_64 (void *file_map, struct stat file_stat,
							 const char *file, int fd)
{
	const char *error_msg = ": file format not recognized\n";
	Elf64_Ehdr *header	  = (Elf64_Ehdr *)file_map;

	// check file type (ET_EXEC, ET_DYN, ET_REL are the only valid types see
	// subject)
	if (header->e_type != ET_EXEC || header->e_type != ET_DYN
		|| header->e_type != ET_REL)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	// check endianness
	if (header->e_ident[EI_DATA] != ELFDATA2LSB
		|| header->e_ident[EI_DATA] != ELFDATA2MSB)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	// check version
	if (header->e_ident[EI_VERSION] != EV_CURRENT
		|| header->e_version != EV_CURRENT)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	size_t sections_size = header->e_shnum * sizeof (Elf32_Shdr);

	// check if the start of the section headers is within the file
	if (header->e_shoff >= (size_t)file_stat.st_size)
	{
		ft_putstr_fd ("ft_nm: ", 2);
		ft_putstr_fd ((char *)file, 2);
		ft_putstr_fd (": file too short\n", 2);
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);
	}
	// check if the end of the section headers is within the file
	if (header->e_shoff + sections_size > (size_t)file_stat.st_size)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	// check if there are any sections
	if (header->e_shnum == 0)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	// check if the start of the program headers is within the file
	if (header->e_phoff >= (size_t)file_stat.st_size)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	// check if the section header string table index is within the number of
	// sections
	if (header->e_shstrndx >= header->e_shnum)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	return false;
}

static bool check_header_32 (void *file_map, struct stat file_stat,
							 const char *file, int fd)
{
	const char *error_msg = ": file format not recognized\n";
	Elf32_Ehdr *header	  = (Elf32_Ehdr *)(file_map);

	// if (header->e_type != ET_EXEC || header->e_type != ET_DYN
	// 	|| header->e_type != ET_REL)
	// 	return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	if (header->e_type != ET_EXEC || header->e_type != ET_DYN
		|| header->e_type != ET_REL)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	if (header->e_ident[EI_DATA] != ELFDATA2LSB
		|| header->e_ident[EI_DATA] != ELFDATA2MSB)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	if (header->e_ident[EI_VERSION] != EV_CURRENT
		|| header->e_version != EV_CURRENT)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	size_t sections_size = header->e_shnum * sizeof (Elf32_Shdr);

	if (header->e_shoff >= (size_t)file_stat.st_size)
	{
		ft_putstr_fd ("ft_nm: ", 2);
		ft_putstr_fd ((char *)file, 2);
		ft_putstr_fd (": file too short\n", 2);
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);
	}

	if (header->e_shoff + sections_size > (size_t)file_stat.st_size)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	if (header->e_shnum == 0)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	if (header->e_phoff >= (size_t)file_stat.st_size)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	if (header->e_shstrndx >= header->e_shnum)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);

	return false;
}

bool check_header (void *file_map, struct stat file_stat, const char *file,
				   int fd)
{
	const char	  *error_msg = ": file format not recognized\n";
	unsigned char *e_ident	 = (unsigned char *)file_map;

	// check if the file is an ELF file (we can also EI_MAG0, EI_MAG1, EI_MAG2,
	// EI_MAG3 it's the same as ELFMAG)
	if (ft_memcmp (e_ident, ELFMAG, SELFMAG) != 0)
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);
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
		return cleanup_and_report (file, fd, file_map, error_msg, page_size);
	return false;
}