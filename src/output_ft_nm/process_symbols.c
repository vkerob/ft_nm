#include "../../includes/ft_nm.h"

static int compare_symbols (const char *symA, const char *symB)
{
	size_t i = 0, j = 0;

	if (symA == NULL || symB == NULL)
		return 0;

	while (symA[i] != '\0' && symB[j] != '\0')
	{
		while (symA[i] != '\0' && !ft_isalnum ((unsigned char)symA[i]))
			i++;
		while (symB[j] != '\0' && !ft_isalnum ((unsigned char)symB[j]))
			j++;

		// if one of the strings is finished
		if (symA[i] == '\0' || symB[j] == '\0')
			break;

		char normalizedA = ft_tolower ((unsigned char)symA[i]);
		char normalizedB = ft_tolower ((unsigned char)symB[j]);
		if (normalizedA != normalizedB)
			return normalizedA - normalizedB;

		i++;
		j++;
	}

	// cleans up the strings ( exemple : "start" and "start%%%" will have the
	// same normalized string)
	while (symA[i] != '\0' && !ft_isalnum ((unsigned char)symA[i]))
		i++;
	while (symB[j] != '\0' && !ft_isalnum ((unsigned char)symB[j]))
		j++;

	// if both strings are empty
	if (symA[i] == '\0' && symB[j] == '\0')
		return ft_strcmp (symA, symB);

	return (unsigned char)symA[i] - (unsigned char)symB[j];
}

static void sort_symbols_by_name (t_symbol_entry *symbols, long symbol_count)
{
	if (!symbols || symbol_count <= 1)
		return;

	for (long i = 0; i < symbol_count - 1; i++)
	{
		for (long j = 0; j < symbol_count - i - 1; j++)
		{
			if (compare_symbols (symbols[j].name, symbols[j + 1].name) > 0)
			{
				// Échange
				t_symbol_entry tmp = symbols[j];
				symbols[j]		   = symbols[j + 1];
				symbols[j + 1]	   = tmp;
			}
		}
	}
}

void ft_putnbr_hex_fd (unsigned long adress, int fd, int width)
{
	char *hex_digits = "0123456789abcdef";
	char  buffer[32];
	int	  i;

	i = 0;
	if (adress == 0)
		buffer[i++] = '0';
	else
	{
		while (adress > 0)
		{
			buffer[i++] = hex_digits[adress % 16];
			adress /= 16;
		}
	}
	while (i < width)
		buffer[i++] = '0';
	while (i--)
		ft_putchar_fd (buffer[i], fd);
}

void print_ft_nm (t_symbol_entry *symbols, long symbol_count, int width)
{
	for (long i = 0; i < symbol_count; i++)
	{
		if (symbols[i].type_char == ' ')
			continue;

		if ((symbols[i].value == NULL || symbols[i].type_char == 'U'
			 || symbols[i].type_char == 'w')
			&& symbols[i].type_char != 'T')
		{
			for (int j = 0; j < width; j++)
				ft_putchar_fd (' ', 1);
		}
		else
			ft_putnbr_hex_fd ((unsigned long)symbols[i].value, 1, width);
		ft_printf (" %c %s\n", symbols[i].type_char, symbols[i].name);
	}
}

void print_reverse_alpha_sort (t_symbol_entry *symbols, long symbol_count,
							   int width)
{
	for (long i = symbol_count; i > 0; i--)
	{
		if (symbols[i - 1].type_char == ' ')
			continue;
		if (symbols[i - 1].value == NULL)
		{
			for (int j = 0; j < width; j++)
				ft_putchar_fd (' ', 1);
		}
		else
			ft_putnbr_hex_fd ((unsigned long)symbols[i - 1].value, 1, width);
		ft_printf (" %c %s\n", symbols[i - 1].type_char, symbols[i - 1].name);
	}
}

void process_symbols (t_symbol_entry **symbols, long symbol_count,
					  const char *file, int nb_files)
{

	int width = is_64 ? 16 : 8; // address width
	// sort the symbols by name alphabetically
	if (no_sort == false)
		sort_symbols_by_name (*symbols, symbol_count);
	if (nb_files > 2)
		printf ("\n%s:\n", file);
	if (reverse_sort)
		print_reverse_alpha_sort (*symbols, symbol_count, width);
	else
		print_ft_nm (*symbols, symbol_count, width);
}