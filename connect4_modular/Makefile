CC = gcc
SRC = $(wildcard src/*.c)
OUT = connect4
PKGCONFIG = allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_main-5 allegro_image-5
CFLAGS = $(shell pkg-config --cflags $(PKGCONFIG))
LDFLAGS = $(shell pkg-config --libs $(PKGCONFIG))

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(SRC) -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)
