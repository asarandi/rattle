CFLAGS  += -O1 -Wall -Werror -Wextra
CFLAGS  += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --static-libs) -lm

src_rattle = rattle.c

src_keyb = keyb.c

rattle:
	$(CC) $(CFLAGS) $(src_rattle) $(LDFLAGS) -o rattle

keyb:
	$(CC) $(CFLAGS) $(src_keyb) $(LDFLAGS) -o keyb
