CFLAGS += -g -Wall -Werror -Wextra
CFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += -g -lm $(shell sdl2-config --libs)

SRC += main.c mystrings.c notes.c parse.c samples.c
OBJ += $(SRC:.c=.o)

rattle: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

keyb: keyb.c
	$(CC) $(CFLAGS) $< $(LDFLAGS) -o $@

format:
	clang-format -i *.c *.h

fclean:
	rm -f $(OBJ) rattle keyb

test: rattle
	./rattle `grep Mozart samples.txt`
