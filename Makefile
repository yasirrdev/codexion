NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = coders/src/main.c \
      coders/src/init.c \
      coders/src/threads.c \
      coders/src/utils.c

OBJ = $(SRC:.c=.o)

INCLUDE = -I coders/include

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
