CFLAGS += -g -Wall -Werror -Wextra
CFLAGS += $(shell sdl2-config --cflags)
LDFLAGS += -g -lm $(shell sdl2-config --libs)

SRC += main.c notes.c samples.c
OBJ += $(SRC:.c=.o)

main: $(OBJ)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

format:
	clang-format -i *.c *.h

fclean:
	$(RM) $(OBJ) main
