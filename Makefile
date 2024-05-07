cc=gcc
CFLAGS=-I. -g -w -lm
DEPS = 
OBJ = spell_checker.o
%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
spell_checker: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)