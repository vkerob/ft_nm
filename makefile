# Variables générales
CC = gcc

# -Wall -Wextra -Werror

CFLAGS = -g

# Répertoires
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

NAME = ft_nm

SRC_DIR = src
OBJ_DIR = obj

INCLUDE_DIR = includes

# Fichiers source et objets
SRC = main.c


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