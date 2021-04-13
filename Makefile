CFLAGS  += -O1 -Wall -Werror -Wextra
CFLAGS  += $(shell sdl2-config --cflags)
LDFLAGS += $(shell sdl2-config --static-libs) -lm

rattle: rattle.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

keyb: keyb.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@
