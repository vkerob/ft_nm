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

extern int	page_size;
extern bool is_64;

extern bool external_only;	/* -g Print external symbols only.  */
extern bool undefined_only; /* -u Print undefined symbols only.  */
extern bool reverse_sort;	/* -r Sort in downward alpha order.  */
extern bool no_sort;		/* -p Don't sort; print syms in order found.  */

typedef struct s_symbol_entry
{
	const char *name;	   // Pointer to the name of the symbol
	void	   *value;	   // Value of the symbol (address)
	char		type_char; // Type (T, U, etc.)
} t_symbol_entry;

/* Error messages */
#define ERR_FORMAT_NOT_RECOGNIZED "file format not recognized\n"
#define ERR_CORRUPT_STRING_TABLE "has a corrupt string table index - ignoring\n"
#define ERR_FILE_TOO_SHORT "file too short\n"
#define ERR_NO_SYMBOLS "no symbols\n"
#define ERR_NO_FILE "No such file or directory\n"
#define ERR_NO_PERM "Permission denied\n"
#define ERR_NO_MEM "Cannot allocate memory\n"
#define ERR_GET_FILE_STATS "Error getting file stats\n"
#define ERR_NEGATIVE_SIZE "has negative size, probably it is too large\n"
#define ERR_NOT_REGULAR "is not an ordinary file\n"
#define ERR_MAP_FAILED "Error mapping file\n"
#define ERR_SECTION_NAME_OUT_OF_RANGE "ELF section name out of range\n"
#define ERR_IS_DIR "is a directory\n"
#define ERR_MALLOC_FAILED "malloc failed\n"

bool display_file (const char *file, int argc);

bool get_fd (const char *file, int *fd);

bool get_file_stat (struct stat *file_stat, const char *file, int *fd);

bool cleanup_and_report (const char *file, int fd, void *file_map,
						 const char *error_msg, size_t map_size);
void print_error_message (const char *prefix, const char *file,
						  const char *msg);

bool get_file_map (void **file_map, struct stat file_stat, const char *file,
				   int fd);

bool check_header (void *file_map, struct stat file_stat, const char *file,
				   int fd);

bool save_symbols (void *file_map, t_symbol_entry **symbols, long *symbol_count,
				   const char *file_name, size_t file_size);

size_t get_symbols_count_64 (Elf64_Ehdr *header, Elf64_Shdr *section_headers,
							 Elf64_Shdr **symtab_section,
							 Elf64_Shdr **strtab_section, size_t file_size);
size_t get_symbols_count_32 (Elf32_Ehdr *header, Elf32_Shdr *section_headers,
							 Elf32_Shdr **symtab_section,
							 Elf32_Shdr **strtab_section, size_t file_size);

void set_symbols_64 (Elf64_Ehdr *header, Elf64_Shdr *section_headers,
					 t_symbol_entry *symbols, Elf64_Shdr *symtab_section,
					 Elf64_Shdr *strtab_section);
void set_symbols_32 (Elf32_Ehdr *header, Elf32_Shdr *section_headers,
					 t_symbol_entry *symbols, Elf32_Shdr *symtab_section,
					 Elf32_Shdr *strtab_section);

char get_symbol_type_char_64 (Elf64_Sym sym, Elf64_Shdr *sections);
char get_symbol_type_char_32 (Elf32_Sym sym, Elf32_Shdr *sections);

bool is_unvalid_symbol_64 (Elf64_Sym sym, const char *strtab,
						   Elf64_Shdr *sections);
bool is_unvalid_symbol_32 (Elf32_Sym sym, const char *strtab,
						   Elf32_Shdr *sections);

bool is_external_64 (Elf64_Sym sym);
bool is_external_32 (Elf32_Sym sym);

void process_symbols (t_symbol_entry **symbols, long symbol_count,
					  const char *file, int argc);

#endif