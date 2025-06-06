CC = gcc
CFLAGS = -Wall -std=c11 `pkg-config --cflags allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5`
LDFLAGS = `pkg-config --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5`
SRC = main.c game.c fileio.c gui.c
OBJ = $(SRC:.c=.o)
EXEC = connect4.exe

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f *.o $(EXEC)
