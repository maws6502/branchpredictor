CC=gcc
CFLAGS=-I. -Wall -Wno-parentheses --std=c89 # fight me
DEPS = predictor.h
OBJ = predictor.o dynamic_bm_gshare.o dynamic_bm.o static_taken.o static_not_taken.o dynamic_tournament.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

predictor: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o predictor