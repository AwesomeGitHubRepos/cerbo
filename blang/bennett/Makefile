TARGS = bytes vasmc vas-parser

all : $(TARGS)


bytes : bytes.cc
	g++ $^ -o $@


#mcutils.o : mcutils.import.scm
mcutils.so : mcutils.scm
	csc -s $^

vas-parser.o : vas-parser.scm
	csc -c $^

#vas-parser : mcutils.o vas-parser.o
vas-parser : vas-parser.scm mcutils.scm
	csc $< -o $@

vasmc : vasmc.scm
	csc $^

clean :
	rm -f $(TARGS) *.o *.so
