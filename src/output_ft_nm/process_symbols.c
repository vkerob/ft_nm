#include "../../includes/ft_nm.h"


static int compare_symbols(const char *symA, const char *symB) {
    size_t i = 0, j = 0;

    while (symA[i] != '\0' && symB[j] != '\0') 
	{
        while (symA[i] != '\0' && !ft_isalnum((unsigned char)symA[i]))
            i++;
        while (symB[j] != '\0' && !ft_isalnum((unsigned char)symB[j]))
            j++;

        // if one of the strings is finished
        if (symA[i] == '\0' || symB[j] == '\0')
            break;

        char normalizedA = ft_tolower((unsigned char)symA[i]);
        char normalizedB = ft_tolower((unsigned char)symB[j]);
        if (normalizedA != normalizedB)
            return normalizedA - normalizedB;

        i++;
        j++;
    }

    // cleans up the strings ( exemple : "start" and "start%%%" will have the same normalized string)
    while (symA[i] != '\0' && !ft_isalnum((unsigned char)symA[i]))
        i++;
    while (symB[j] != '\0' && !ft_isalnum((unsigned char)symB[j]))
        j++;

	// if both strings are empty
	if (symA[i] == '\0' && symB[j] == '\0')
        return ft_strcmp(symA, symB);

    return (unsigned char)symA[i] - (unsigned char)symB[j];
}



static void	sort_symbols_by_name(t_symbol_entry *symbols, size_t symbol_count)
{
    if (!symbols || symbol_count <= 1)
        return;


    for (size_t i = 0; i < symbol_count - 1; i++)
    {
        for (size_t j = 0; j < symbol_count - i - 1; j++)
        {
            if (compare_symbols(symbols[j].name, symbols[j + 1].name) > 0)
            {
                // Échange
                t_symbol_entry tmp = symbols[j];
                symbols[j] = symbols[j + 1];
                symbols[j + 1] = tmp;
            }
        }
    }
}

void	ft_putnbr_hex_fd(unsigned long n, int fd, int width)
{
	char	*hex_digits = "0123456789abcdef";
	char	buffer[32];  // suffisant pour 64 bits
	int		i;

	i = 0;
	// Si n est 0, on met un seul '0'
	if (n == 0)
		buffer[i++] = '0';
	else
	{
		while (n > 0)
		{
			buffer[i++] = hex_digits[n % 16];
			n /= 16;
		}
	}
	// Calcul du nombre de zéros à ajouter pour atteindre la largeur souhaitée
	while (i < width)
		buffer[i++] = '0';
	// Affichage dans l'ordre inverse (le buffer contient le chiffre le moins significatif en premier)
	while (i--)
		ft_putchar_fd(buffer[i], fd);
}

static void	print_symbols_64(t_symbol_entry *symbols, size_t symbol_count)
{
	for (size_t i = 0; i < symbol_count; i++)
	{
		printf("test1\n");
		if (symbols[i].value == NULL && symbols[i].type_char == 'U')
			ft_putstr_fd("                ", 1);
		else
			ft_putnbr_hex_fd((unsigned long)symbols[i].value, 1, 16);
		ft_printf(" %c %s\n", symbols[i].type_char, symbols[i].name);
	}
}

static void	print_symbols_32(t_symbol_entry *symbols, size_t symbol_count)
{
	for (size_t i = 0; i < symbol_count; i++)
	{
		if (symbols[i].type_char == ' ')
			continue;
		if (symbols[i].value == NULL && symbols[i].type_char == 'U')
			ft_putstr_fd("        ", 1);
		else
			ft_putnbr_hex_fd((unsigned long)symbols[i].value, 1, 8);

		ft_printf(" %c %s\n", symbols[i].type_char, symbols[i].name);
	}
}


void	process_symbols(t_symbol_entry **symbols, size_t symbol_count, const char *file, int argc)
{
	// sort the symbols by name alphabetically
	sort_symbols_by_name(*symbols, symbol_count);
	if (argc > 2)
		printf("\n%s:\n", file);
	if (is_64)
		print_symbols_64(*symbols, symbol_count);
	else
		print_symbols_32(*symbols, symbol_count);
	free(*symbols);
}