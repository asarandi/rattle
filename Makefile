CFLAGS  += -O1 -Wall -Werror -Wextra
CFLAGS  += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --static-libs) -lm

src = rattle.c

rattle:
	$(CC) $(CFLAGS) $(src) $(LDFLAGS) -o rattle

