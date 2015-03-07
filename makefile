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

buildinfo.o:
	@echo "Generating build info..."
	@echo $(shell git log -1 --pretty=oneline)
	@echo "#include \"buildinfo.h\"" > $(srcdir)/$*.c
	@echo "const char *buildVersion = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(version))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildCommit = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git log -1 --pretty=%H))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildCommitShort = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git log -1 --pretty=%h))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildCommitDate = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git log -1 --pretty=%cD))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildDate = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell date -R))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildAuthor = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git log -1 --pretty="%an <%ae>"))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildBranch = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git symbolic-ref HEAD))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildSubject = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git log -1 --pretty=%s))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildBody = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git log -1 --pretty=%b))))\";" >> $(srcdir)/$*.c
	@echo "const char *buildInfo = \"$(subst \n,\\n,$(subst ",\\\",$(subst \\,\\\\,$(shell git log -1 --pretty=oneline))))\";" >> $(srcdir)/$*.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $(objdir)/$*.o $(srcdir)/$*.c

%.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $(objdir)/$*.o $(srcdir)/$*.c

main.exe: $(_OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)
	
.PHONY: all
all: main.exe
	
.PHONY: clean
clean:
	rm -f $(objdir)/*.o main.exe core *.core