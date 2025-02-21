# Variables générales
CC = clang

# -Wall -Wextra -Werror

CFLAGS = -g -m32

# Répertoires
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

NAME = ft_nm

SRC_DIR = src
OBJ_DIR = obj

INCLUDE_DIR = includes

# Fichiers source et objets
SRC = ft_nm.c \
		display_file.c \
		files_handler/get_fd.c \
		files_handler/get_file_stat.c \
		files_handler/get_file_map.c \
		symbols_handler/save_symbols.c \
		symbols_handler/set_symbols.c \
		symbols_handler/get_symbols_count.c \
		symbols_handler/get_symbol_type_char.c \
		symbols_handler/is_external.c \
		symbols_handler/is_unvalid_symbol.c \
		output_ft_nm/process_symbols.c


OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

all: $(NAME)

$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) $(OBJ) $(LIBFT) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -I $(INCLUDE_DIR) -c $< -o $@

$(LIBFT):
	make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -rf $(OBJ_DIR)
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re