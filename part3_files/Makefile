IDIR =./include
CC=g++
CFLAGS=-std=c++11 -w -O3 -march=native -I$(IDIR)

ODIR=./obj
LIBS=-lm -pthread
SRC = ./src

_DEPS = functional_cache.h knobs.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = functional_cache.o part3.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRC)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

part3: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ part3
