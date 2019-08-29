CC=gcc
CFLAGS=-g -wall
EOBJS=ecu.o
ECU=ecu
COBJS=controller.o
CONT=controller

all: $(ECU) $(CONT)

ecu.o: ecu.c ecu.h
	$(CC) -c ecu.c -lm

$(ECU):$(EOBJS)
	$(CC) -o ecu ecu.o -lm

controller.o: controller.c ecu.h
	$(CC) -c controller.c

$(CONT):$(COBJS)
	$(CC) -o controller controller.o

clean:
	rm -f *.o
	rm -f $(ECU)
	rm -f $(CONT)
