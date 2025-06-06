
CC=gcc
CFLAGS=`pkg-config --cflags allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_main-5 allegro_image-5` -Wall -Iinclude
LDFLAGS=`pkg-config --libs allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_main-5 allegro_image-5`
SRC=$(wildcard src/*.c)
OBJ=$(SRC:.c=.o)
TARGET=connect4

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(OBJ) $(TARGET)
