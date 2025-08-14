CFLAGS += -Wall -Werror -Wextra
CFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += -lm $(shell sdl2-config --libs)

rattle: main.o mystrings.o notes.o parse.o waves.o
	$(CC) -o $@ $^ $(LDFLAGS)

keyb: keyb.o notes.o waves.o
	$(CC) -o $@ $^ $(LDFLAGS)

format:
	clang-format -i *.c *.h

fclean:
	rm -f *.o rattle keyb

test: rattle
	./rattle -v -1 "NokiaTun:d=4,o=5,b=225:8e6,8d6,f#,g#,8c#6,8b,d,e,8b,8a,c#,e,2a,8p"
