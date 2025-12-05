CXX = g++
CXXFLAGS = -std=c++17 -Iinclude `pkg-config --cflags allegro-5 allegro_image-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5` -O2
LDFLAGS = `pkg-config --libs allegro-5 allegro_image-5 allegro_primitives-5 allegro_font-5 allegro_ttf-5`

SRCS = $(wildcard src/*.cpp)
OBJS = $(SRCS:.cpp=.o)

all: connect4_cpp

connect4_cpp: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f src/*.o connect4_cpp

.PHONY: all clean
