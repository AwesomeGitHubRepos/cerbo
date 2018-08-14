TARGS = bytes vasmc vas-parser

all : $(TARGS)


bytes : bytes.cc
	g++ $^ -o $@

vas-parser: vas-parser.scm
	csc $^

vasmc : vasmc.scm
	csc $^

clean :
	rm -f $(TARGS)
