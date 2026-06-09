NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRC = src/main.c \
      src/init.c \
      src/threads.c \
      src/dongles.c \
      src/heap.c \
      src/scheduler.c \
      src/monitor.c \
      src/routine.c \
      src/utils.c

OBJ = $(SRC:.c=.o)

INCLUDE = -I include

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
