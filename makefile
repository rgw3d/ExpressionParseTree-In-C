srcdir = src
objdir = obj
includedir = include

CC = gcc
LIBS =
CDEBUG = -g
CFLAGS = $(CDEBUG) -I. -I$(includedir) -I$(srcdir) -Wall
INCLUDE =

_DEPS = lexer.h token.h parser.h stringstack.h input.h
DEPS = $(patsubst %,$(includedir)/%,$(_DEPS))

_SRCS = main.c lexer.c token.c parser.c stringstack.c input.c
SRCS = $(patsubst %,$(srcdir)/%,$(_SRCS))

_OBJS = $(_SRCS:.c=.o)
OBJS = $(patsubst %,$(objdir)/%,$(_OBJS))

%.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $(objdir)/$*.o $(srcdir)/$*.c

main.exe: $(_OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
	
.PHONY: all
all: main.exe
	
.PHONY: clean
clean:
	rm -f $(objdir)/*.o main.exe core *.core