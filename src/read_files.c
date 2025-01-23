# include "../includes/ft_nm.h"


void	*verify_file(t_data data, const char *file, void *last_header, bool header_mappped)
{
	int			fd;
	struct stat	file_stat;
	void		*header;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("ft_nm :%s: %s\n", file, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (fstat(fd, &file_stat) < 0)
	{
		perror("Error getting file stats");
		exit(EXIT_FAILURE);
	}
	if (file_stat.st_size == 0)
	{
		// indiquer que c'est un fichier a pas traiter
		close(fd);
		return ; // code 2
	}
	if (file_stat.st_size < sizeof(Elf64_Ehdr))
	{
		ft_printf("ft_nm : %s: File format not recognized\n", file);
		close(fd);
		return ; // code 3
	}
	if (!header_mappped)
	{
		header = mmap(NULL, page_size, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, fd, 0);
		if (header == MAP_FAILED)
		{
			perror("Error mapping file");
			exit(EXIT_FAILURE);
		}
		header_mappped = true;
	}
	else
	{
		header = mmap(last_header, page_size, PROT_READ, MAP_PRIVATE | MAP_FIXED, fd, 0);
		if (header == MAP_FAILED)
		{
			perror("Error mapping file");
			exit(EXIT_FAILURE);
		}
	}
	// fichier valide
	return ; // code 0
	ft_printf("File: %s\n", file);
	ft_printf("File size: %i\n", file_stat.st_size);

}


void	verify_all_files(t_data data, int argc, const char **argv,  void *last_header, bool header_mappped)
{
	void	*header;
	int		i;

	i = 1;
	while (i < argc)
	{
		header = verify_file(data, argv[i], last_header, header_mappped);
		last_header = header;
		i++;
	}
}


void	verify_entry(t_data data, int argc, const char **argv)
{
	char	*file = "a.out";
	bool	header_mappped = false;
	void	*last_header = NULL;

	if (argc <= 2) // if there is only one argument or no argument
	{
		if (argc == 2)
			file = (char *)argv[1];
		verify_file(data, file, last_header, header_mappped);
		return ;
	}
	// if there are more than 2 arguments
	verify_all_files(data, argc, argv, last_header, header_mappped);
	
}


void	read_files(t_data data, int argc, const char **argv)
{
	verify_entry(data, argc, argv);
}