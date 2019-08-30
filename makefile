CC=gcc
CFLAGS=-g -wall
EOBJS=ecu.o
ECU=ecu
GOBJS=engine.o
ENG=engine

all: $(ECU) $(ENG)

ecu.o: ecu.c ecu.h
	$(CC) -c ecu.c -lm -pthread

$(ECU):$(EOBJS)
	$(CC) -o ecu ecu.o -lm -pthread

engine.o: engine.c ecu.h
	$(CC) -c engine.c

$(ENG):$(GOBJS)
	$(CC) -o engine engine.o

clean:
	rm -f *.o
	rm -f $(ECU)
	rm -f $(ENG)
