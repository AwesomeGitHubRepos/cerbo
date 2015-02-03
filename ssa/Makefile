UNAME := $(shell uname)
BUILT_SOURCES = *.xxd

CC = gcc
CFLAGS = -ggdb -std=gnu99 
LDFLAGS = -L/usr/local/lib
MAN = ssa.1.gz
PREFIX=/usr/local

OBJS=  \
	commands.o \
	csv.o \
	curly.o \
	data.o \
	epics.o \
	financials.o \
	globals.o \
	htm.o \
	inodes.o \
	logging.o \
	ofx.o \
	parser.o \
	portfolios.o \
	postings.o \
	prolog.o \
	simclist.o \
	utils.o




GOFIC = gofic
SSA = ssa
ifeq ($(UNAME),CYGWIN_NT-6.1)
	CYGL = -largp
	EXEX = .exe
	GOFIC = gofic.exe
	SSA = ssa.exe
endif


BINS = curly globals gofic parser ssa 
VERSION := $(shell date)

.PHONY : all clean globals-test info install leaks tags uninstall profile

all: $(BINS) $(MAN)

clean :
	rm -f core curly globals gofic parser ssa ssa.exe.stackdump tags
	rm -f *.exe
	rm -f *.o *~
	rm -f $(BUILT_SOURCES)
	rm -f $(MAN)
	rm -f callgrind.out.*

# print some sueful information to help construct makefiles
info :
	@echo "UNAME=$(UNAME)"

$(MAN) : ssa.1
	gzip < ssa.1 > $(MAN)

install : $(GOFIC) $(SSA) $(SCRIPTS)
	cp $(GOFIC) $(SSA) $(SCRIPTS) $(PREFIX)/bin
	cp $(MAN) $(PREFIX)/share/man/man1

uninstall :
	(cd $(PREFXI)/in && rm $(SSA) $(GOFIC) $(SCRIPTS))
	(cd $(PREFIX)/share/man/man1 && rm $(MAN))


tags : 
	ctags -R *


%.o : %.c
	# echo a version is $(VERSION)
	$(CC) -DVERSION="\"$(VERSION)\"" -c $(CFLAGS) $< -o $@


curly.xxd : curly.txt
	xxd -i <curly.txt > curly.xxd

curly.o :  curly.c curly.h curly.xxd
	$(CC) $(CFLAGS) -c curly.c  -lpthread -lcurl

curly : curly.c curly.h curly.xxd inodes.o parser.o logging.o
	$(CC) $(CFLAGS) -Dmain_curly=main curly.c inodes.o parser.o logging.o -o curly -lpthread -lcurl -lm

htm.xxd : ssa.htm
	xxd -i <  ssa.htm > htm.xxd

htm.o : htm.c htm.xxd

fetch-script.xxd : fetch-script.sh
	xxd -i < fetch-script.sh > fetch-script.xxd

#http.o : http.c fetch-script.xxd

ssa : ssa.o $(OBJS)
	# echo ssa version is $(VERSION)
	$(CC) ssa.o $(OBJS) $(CFLAGS) -lm $(CYGL) $(LDFLAGS) -lpthread -lcurl -o ssa



gofic : gofic.o curly.o inodes.o parser.o logging.o
	$(CC) curly.o gofic.o  inodes.o parser.o logging.o $(CFLAGS) -lm $(CYGL) $(LDFLAGS) -lpthread -lcurl -o gofic

globals : globals.c simclist.o utils.o
	$(CC) -Dmain_globals=main globals.c simclist.o utils.o -o globals

globals-test :
	valgrind --leak-check=yes --show-leak-kinds=all ./globals

parser: parser.c parser.h
	$(CC) -Dmain_parser=main parser.c -o parser

leaks :
	valgrind --leak-check=yes --show-leak-kinds=all ./ssa

profile :
	valgrind --tool=callgrind ./ssa -s
	echo Now run kcachegrind
