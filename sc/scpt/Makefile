.PHONY: clean install

scpt : scpt.scm
	bigloo -o scpt scpt.scm

clean :
	rm -f scpt *~ *.exe *.o

install :
	cp scpt /usr/local/bin

