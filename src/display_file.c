#include "../includes/ft_nm.h"

bool display_file(const char *file, int nb_files)
{
	int fd;
	struct stat file_stat;
	void *file_map;
	t_symbol_entry *symbols = NULL;
	size_t symbol_count = 0;

	if (get_fd(file, &fd))
		return true; // error

	if (get_file_stat(&file_stat, file, &fd))
		return true; // error

	if (get_file_map(&file_map, file_stat, file, fd))
		return true; // error

	if (save_symbols(file_map, &symbols, &symbol_count, file))
	{
		munmap(file_map, file_stat.st_size);
		close(fd);
		return true; // error
	}

	process_symbols(&symbols, symbol_count, file, nb_files);
	free(symbols);
	munmap(file_map, file_stat.st_size);
	close(fd);

	return false;
}
