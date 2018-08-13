all : bytes vasmc


bytes : bytes.cc
	g++ $^ -o $@

vasmc : vasmc.scm
	csc vasmc.scm

clean :
	rm -f bytes vasmc
