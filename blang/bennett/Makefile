TARGS = bytes vasmc

all : $(TARGS)


bytes : bytes.cc
	g++ $^ -o $@

vasmc : vasmc.scm
	csc vasmc.scm

clean :
	rm -f $(TARGS)
