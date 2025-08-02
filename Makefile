CC = clang

CFLAGS = -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -Werror
CFLAGS += $(shell pkg-config --cflags glew sdl3 cglm)

LDFLAGS = $(shell pkg-config --libs glew sdl3 cglm)
LDFLAGS += -framework OpenGL

# %.o: %.c
# 	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

PHONY: clean
clean:
	rm -rf main *.o *.dSYM
