CFLAGS += -Wall -Werror -Wextra
CFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += -lm $(shell sdl2-config --libs)

main: main.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

format: main.c
	clang-format -i $<
