CFLAGS  += -O1 -Wall -Werror -Wextra
CFLAGS  += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --libs)

src = rattle.c

rattle:
	$(CC) $(CFLAGS) $(src) $(LDFLAGS) -o rattle

