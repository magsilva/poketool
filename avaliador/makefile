#
#
# Makefile to produce the executable code for the AVALIADOR module
#
#

# INSERIDO POR EYN EM 09/10/96
HOME = /home1


OBJS = itoa.o mainava2.o aval1.o aval2.o aval3.o rotsem2.o

BIN = ${HOME}/poketool/bin

INCL = ${HOME}/poketool/include

POKETABS = ${HOME}/poketool/bin/poketabs

CFLAGS = -c -I$(INCL)

avaliador: $(OBJS)
	$(CC) $(OBJS) -lm -o avaliador
	cp avaliador $(BIN)
	cp pathsd.tes $(POKETABS)

mainava2.o: mainava2.c $(INCL)/haval.h $(INCL)/hrotsem.h $(INCL)/util.h
	$(CC) $(CFLAGS) mainava2.c

aval1.o: aval1.c $(INCL)/haval.h $(INCL)/hrotsem.h $(INCL)/util.h
	$(CC) $(CFLAGS) aval1.c

aval2.o: aval2.c $(INCL)/haval.h $(INCL)/hrotsem.h $(INCL)/util.h
	$(CC) $(CFLAGS) aval2.c

aval3.o: aval3.c $(INCL)/haval.h $(INCL)/hrotsem.h $(INCL)/util.h
	$(CC) $(CFLAGS) aval3.c

rotsem2.o: rotsem2.c $(INCL)/haval.h $(INCL)/hrotsem.h $(INCL)/util.h
	$(CC) $(CFLAGS) rotsem2.c

itoa.o: itoa.c
	$(CC) $(CFLAGS) itoa.c

