exe=scx2lol

.PHONY: clean install

$(exe) : $(exe).scm
	csc -o $(exe) $(exe).scm

clean :
	rm -f $(exe) *~ *.exe *.o

install :
	cp $(exe) ~/.local/bin

