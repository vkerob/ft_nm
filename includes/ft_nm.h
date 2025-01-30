# ifndef FT_NM_H

#include "../libft/libft.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <elf.h>
#include <string.h>
# include <stdbool.h>

extern int	page_size;
extern int	retval;


typedef struct s_symbol_entry
{
    const char *name;       // Pointer to the name of the symbol
    Elf64_Addr value;       // Value of the symbol (address)
    char        type_char;  // Type (T, U, etc.)
} t_symbol_entry;

bool	display_files(const char *file);
bool	save_symbols_64(void *file_map, Elf64_Shdr *sections, Elf64_Half sections_count, t_symbol_entry **symbols, size_t *symbol_count);

#endif