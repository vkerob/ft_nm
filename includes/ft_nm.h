#ifndef FT_NM_H

#include "../libft/libft.h"
#include <elf.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

extern int page_size;
extern bool is_64;

extern bool print_debug_syms; /* -a Print debugger-only symbols too. */
extern bool external_only;    /* -g Print external symbols only.  */
extern bool undefined_only;   /* -u Print undefined symbols only.  */
extern bool reverse_sort;     /* -r Sort in downward alpha order.  */
extern bool no_sort;          /* -p Don't sort; print syms in order found.  */

typedef struct s_symbol_entry
{
    const char *name; // Pointer to the name of the symbol
    void *value;      // Value of the symbol (address)
    char type_char;   // Type (T, U, etc.)
} t_symbol_entry;

bool display_file(const char *file, int argc);
bool get_fd(const char *file, int *fd);
bool get_file_stat(struct stat *file_stat, const char *file, int *fd);
bool get_file_map(void **file_map, struct stat file_stat, const char *file,
                  int fd);
bool save_symbols(void *file_map, t_symbol_entry **symbols,
                  size_t *symbol_count, const char *file_name);
void process_symbols(t_symbol_entry **symbols, size_t symbol_count,
                     const char *file, int argc);

size_t get_symbols_count_64(void *file_map, Elf64_Shdr *sections,
                            Elf64_Half sections_count);
size_t get_symbols_count_32(void *file_map, Elf32_Shdr *sections,
                            Elf32_Half sections_count);

char get_symbol_type_char_64(Elf64_Sym sym, Elf64_Shdr *sections);
char get_symbol_type_char_32(Elf32_Sym sym, Elf32_Shdr *sections);

void set_symbols_64(void *file_map, Elf64_Shdr *sections,
                    t_symbol_entry *symbols, Elf64_Half sections_count);
void set_symbols_32(void *file_map, Elf32_Shdr *sections,
                    t_symbol_entry *symbols, Elf32_Half sections_count);

bool is_external_64(Elf64_Sym sym);
bool is_external_32(Elf32_Sym sym);

bool is_unvalid_symbol_64(Elf64_Sym sym, size_t current_index,
                          Elf64_Sym *symtab, const char *strtab,
                          size_t total_symbols, Elf64_Shdr *sections);
bool is_unvalid_symbol_32(Elf32_Sym sym, size_t current_index,
                          Elf32_Sym *symtab, const char *strtab,
                          size_t total_symbols, Elf32_Shdr *sections);

#endif