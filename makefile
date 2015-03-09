srcdir = src
objdir = obj
includedir = include

version = 1.0.0

CC = gcc
LIBS =
CDEBUG = -g
CFLAGS = $(CDEBUG) -I. -I$(includedir) -I$(srcdir) -Wall -std=c11
INCLUDE =

_DEPS = lexer.h token.h parser.h parsergen.h stringstack.h input.h buildinfo.h memwrapper.h
DEPS = $(patsubst %,$(includedir)/%,$(_DEPS))

_SRCS = main.c $(_DEPS:.h=.c)
SRCS = $(patsubst %,$(srcdir)/%,$(_SRCS))

_OBJS = $(_SRCS:.c=.o)
OBJS = $(patsubst %,$(objdir)/%,$(_OBJS))

.PHONY: all
all: obj main.exe
	
obj:
	mkdir obj

buildinfo.o:
	@echo "Generating build info..."
	@echo $(shell git log -1 --abbrev-commit --pretty=oneline)
	@echo "#include \"buildinfo.h\"" > $(srcdir)/$*.c
	@echo "const char *buildVersion = \"$(subst ",\\\",$(version)))\";" >> $(srcdir)/$*.c
	@echo "const char *buildCommit = \"$(subst ",\\\",$(shell git log -1 --pretty=%H))\";" >> $(srcdir)/$*.c
	@echo "const char *buildCommitShort = \"$(subst ",\\\",$(shell git log -1 --pretty=%h))\";" >> $(srcdir)/$*.c
	@echo "const char *buildCommitDate = \"$(subst ",\\\",$(shell git log -1 --pretty=%cD))\";" >> $(srcdir)/$*.c
	@echo "const char *buildDate = \"$(subst ",\\\",$(shell date -R))\";" >> $(srcdir)/$*.c
	@echo "const char *buildAuthor = \"$(subst ",\\\",$(shell git log -1 --pretty="%an <%ae>"))\";" >> $(srcdir)/$*.c
	@echo "const char *buildBranch = \"$(subst ",\\\",$(shell git symbolic-ref HEAD))\";" >> $(srcdir)/$*.c
	@echo "const char *buildSubject = \"$(subst ",\\\",$(shell git log -1 --pretty=%s))\";" >> $(srcdir)/$*.c
	@echo "const char *buildBody = \"$(subst ",\\\",$(shell git log -1 --pretty=%b))\";" >> $(srcdir)/$*.c
	@echo "const char *buildInfo = \"$(subst ",\\\",$(shell git log -1 --pretty=fuller))\";" >> $(srcdir)/$*.c
#	$(CC) $(CFLAGS) $(INCLUDE) -c -o $(objdir)/$*.o $(srcdir)/$*.c
	$(CC) $(CFLAGS) -c -o $(objdir)/$*.o $(srcdir)/$*.c
	
$(objdir)/%.o: $(srcdir)/%.c
#	$(CC) $(CFLAGS) $(INCLUDE) -c -o $(objdir)/$*.o $(srcdir)/$*.c
	$(CC) $(CFLAGS) -c -o $(objdir)/$*.o $(srcdir)/$*.c

main.exe: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
	
.PHONY: clean
clean:
	rm -f $(objdir)/*.o main.exe core *.core
	rm -r obj
