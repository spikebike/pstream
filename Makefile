
#define CC for your compiler and OPT for your compiler options

#Intel's compiler for use with intel Xeon Phi 5110P
#CC=icc
#OPT   = -O3 -DUSEAFFINITY -mmic
#LIBS = -lpthread

# with GCC on most anything
#CC = gcc -std=gnu99
OPT = -pedantic -Wall -DUSEAFFINITY -DUSENUMA -O4 -pedantic -Wall -fargument-noalias-anything -fstrict-aliasing
#CC=icc -O3
#linux options
CC = gcc
OPT = -O3 -DCNT -DUSEAFFINITY -DUSENUMA -pedantic -Wall
LIBS = -lpthread -lnuma

#OSX options
#CC = cc
#OPT = -O3 -pedantic -Wall
#LIBS = -lpthread 

SRCFILES=pstream.c

OBJFILES=pstream.o
                                                                                    
pstream: $(OBJFILES) Makefile
	$(CC) -o pstream $(OPT) $(OBJFILES) $(LIBS)

.c.o:
	$(CC) -c $(OPT) $*.c
 
clean:
	/bin/rm -f *.o 

#assumes gnu indent
indent:
	indent -ut -bli0 -ts4 -i3 -ts3 -c3 pstream.c
