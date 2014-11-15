CFLAGS=-ggdb -O0 -Wall

# Needed for cygwin:
#LIBB = -L/usr/local/lib
#INC = -I/usr/local/include

#LIBS = -lgsl -lgslcblas -lblas
# -ltecla

.PHONY: clean install

stats: mcstats.o stats.o lex.yy.o 
	gcc  stats.o mcstats.o lex.yy.o  -lm -o stats

clean:
	rm -f *o stats lex.yy.c help.c *~

lex.yy.c: stats.lex stats.h
	flex stats.lex

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

mcstats.o : mcstats.c mcstats.h

stats.o: stats.c stats.h
	gcc -c stats.c


#help.c : help.txt
#	# http://www.linuxjournal.com/content/embedding-file-executable-aka-hello-world-version-5967
#	cat help.txt | ( echo "unsigned char help_text[] = {"; xxd -i; echo ", 0x00 };" ) > help.c

#help.o: help.c

install: stats
	#mkdir -p ~/.local/bin
	cp stats /usr/local/bin
