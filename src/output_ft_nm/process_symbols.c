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

static void	print_symbols(t_symbol_entry *symbols, size_t symbol_count)
{
	for (size_t i = 0; i < symbol_count; i++)
	{
		if (symbols[i].type_char == ' ')
			continue;
		if (symbols[i].value == 0)
			printf("                ");
		else
			printf("%016lx", (unsigned long)symbols[i].value);
		printf(" %c ", symbols[i].type_char);
		printf("%s\n", symbols[i].name);
	}
}


void	process_symbols(t_symbol_entry **symbols, size_t symbol_count, const char *file, int argc)
{
	// sort the symbols by name alphabetically
	sort_symbols_by_name(*symbols, symbol_count);
	if (argc > 2)
		printf("\n%s:\n", file);
	print_symbols(*symbols, symbol_count);
	free(*symbols);
}