#include "../includes/ft_nm.h"

bool display_file(const char *file, int nb_files)
{
	int fd;
	struct stat file_stat;
	void *file_map;
	t_symbol_entry *symbols = NULL;
	long symbol_count = 0;

	if (get_fd(file, &fd))
		return true; // error

	if (get_file_stat(&file_stat, file, &fd))
		return true; // error

	if (get_file_map(&file_map, file_stat, file, fd))
		return true; // error

	if (check_header(file_map, file_stat, file, fd))
		return true; // error

	if (save_symbols(file_map, &symbols, &symbol_count, file,
		file_stat.st_size))
		return cleanup_and_report(file, fd, &file_map, "", file_stat.st_size);

	process_symbols(&symbols, symbol_count, file, nb_files);
	close(fd);
	free(symbols);
	munmap(file_map, file_stat.st_size);

	return false;
}
