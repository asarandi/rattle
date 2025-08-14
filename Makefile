CFLAGS += -Wall -Werror -Wextra
CFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += -lm $(shell sdl2-config --libs)

SRC += main.c mystrings.c notes.c parse.c waves.c
OBJ += $(SRC:.c=.o)

rattle: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

keyb: keyb.c
	$(CC) -o $@ $(CFLAGS) $< $(LDFLAGS)

format:
	clang-format -i *.c *.h

fclean:
	rm -f $(OBJ) rattle keyb

test: rattle
	./rattle -v -1 "NokiaTun:d=4,o=5,b=225:8e6,8d6,f#,g#,8c#6,8b,d,e,8b,8a,c#,e,2a,8p"
