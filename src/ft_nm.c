#include "../includes/ft_nm.h"

int page_size;
bool is_64;

bool external_only = false;	   /* -g Print external symbols only.  */
bool undefined_only = false;   /* -u Print undefined symbols only.  */
bool reverse_sort = false; /* -r Sort in downward(alpha or numeric) order.  */
bool no_sort = false;	   /* -p Don't sort; print syms in order found.  */

bool is_handle_option(char c)
{
	return (c == 'g' || c == 'p' || c == 'r' || c == 'u');
}

int set_options(int argc, const char **argv)
{
	int i = 1;
	int nb_files = argc - 1;

	while (i < argc)
	{
		// If the argument is an option (starting with '-' and is not a single
		// '-')
		if (argv[i][0] == '-' && ft_strlen(argv[i]) > 1)
		{
			for (size_t j = 1; j < ft_strlen(argv[i]); j++)
			{
				if (argv[i][j] == 'g' && !undefined_only)
					external_only = true;
				else if (argv[i][j] == 'p')
				{
					reverse_sort = false;
					no_sort = true;
				}
				else if (argv[i][j] == 'r' && !no_sort)
					reverse_sort = true;
				else if (argv[i][j] == 'u')
				{
					external_only = false;
					undefined_only = true;
				}
				else if (!is_handle_option(argv[i][j]))
				{
					ft_putstr_fd("ft_nm: unknown option -- ", 2);
					ft_putchar_fd(argv[i][j], 2);
					ft_putchar_fd('\n', 2);
					ft_putstr_fd("usage: ft_nm [option(s)] [file(s)]\n", 2);
					exit(1);
				}
			}
			nb_files--;
		}
		i++;
	}
	return (nb_files);
}

char **get_files(int argc, const char **argv, int nb_files)
{
	char **files;
	int file_index = 0;

	files = malloc(sizeof(char *) * nb_files);
	if (!files)
	{
		ft_putstr_fd("ft_nm: memory allocation error\n", 2);
		exit(EXIT_FAILURE);
	}
	for (int i = 1; i < argc; i++)
	{
		// If the argument is not an option (not starting with '-' or is a
		// single '-')
		if (!(argv[i][0] == '-' && ft_strlen(argv[i]) > 1))
		{
			files[file_index] = (char *)argv[i];
			file_index++;
		}
	}
	return (files);
}

int main(int argc, const char **argv)
{
	int i = 1;
	bool retval = true;
	int nb_files;
	char **files;

	page_size = getpagesize();

	nb_files = set_options(argc, argv);

	if (nb_files == 0)
		return (display_file("a.out", 2));

	files = get_files(argc, argv, nb_files);

	i = 0;
	while (i < nb_files)
	{
		display_file(files[i], nb_files);
		i++;
	}

	free(files);
	return retval;
}
