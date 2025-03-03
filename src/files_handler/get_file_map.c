#include "../../includes/ft_nm.h"

static bool cleanup_and_report(const char *file, int fd, void **file_map,
							   char *error_msg, size_t map_size)
{
	if (*file_map && *file_map != MAP_FAILED)
		munmap(*file_map, map_size);
	if (fd != -1)
		close(fd);
	// Modified error output.
	ft_putstr_fd("ft_nm: ", 2);
	ft_putstr_fd((char *)file, 2);
	ft_putstr_fd(error_msg, 2);
	return true;
}

static bool check_header(void **file_map, struct stat file_stat,
						 const char *file, int fd)
{
	*file_map = mmap(NULL, page_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (*file_map == MAP_FAILED)
	{
		cleanup_and_report(file, fd, file_map, "Error mapping file\n",
						   page_size);
		return true;
	}

	unsigned char *e_ident = (unsigned char *)(*file_map);
	if (e_ident[EI_MAG0] != ELFMAG0 || e_ident[EI_MAG1] != ELFMAG1 ||
		e_ident[EI_MAG2] != ELFMAG2 || e_ident[EI_MAG3] != ELFMAG3)
		return cleanup_and_report(file, fd, file_map,
								  ": file format not recognized\n", page_size);
	if (e_ident[EI_CLASS] == ELFCLASS64)
	{
		Elf64_Ehdr *header = (Elf64_Ehdr *)(*file_map);
		if ((header->e_type != ET_EXEC && header->e_type != ET_DYN &&
			header->e_type != ET_REL) ||
			(header->e_ident[EI_DATA] != ELFDATA2LSB && header->e_ident[EI_DATA] != ELFDATA2MSB))
			return cleanup_and_report(file, fd, file_map,
									  ": file format not recognized\n",
									  page_size);                                                                  
		is_64 = true;
	}
	else if (e_ident[EI_CLASS] == ELFCLASS32)
	{
		Elf32_Ehdr *header = (Elf32_Ehdr *)(*file_map);
		if ((header->e_type != ET_EXEC && header->e_type != ET_DYN &&
			header->e_type != ET_REL) ||
			(header->e_ident[EI_DATA] != ELFDATA2LSB && header->e_ident[EI_DATA] != ELFDATA2MSB))
			return cleanup_and_report(file, fd, file_map,
									  ": file format not recognized\n",
									  page_size);
		is_64 = false;
	}
	else
	{
		return cleanup_and_report(file, fd, file_map,
								  ": file format not recognized\n", page_size);
	}

	// if the file is larger than the page size, unmap it
	if (file_stat.st_size > page_size)
	{
		munmap(*file_map, page_size);
		*file_map = NULL;
	}

	return false;
}

bool get_file_map(void **file_map, struct stat file_stat, const char *file,
				  int fd)
{
	if (check_header(file_map, file_stat, file, fd))
		return true;

	// if file_map is not already mapped
	// map it (can be already mapped if the file is smaller than the page size)
	if (!(*file_map))
	{
		*file_map =
			mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		if (*file_map == MAP_FAILED)
		{
			// Modified error output.
			return cleanup_and_report(file, fd, file_map,
									  ": Error mapping file\n",
									  file_stat.st_size);
		}
	}
	return false;
}