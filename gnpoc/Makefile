.PHONY : go

gnpoc : gnpoc.f90
	gfortran gnpoc.f90 -o gnpoc

go :
	rm gnpoc.txt
	emacs gnpoc.txt
	gnpoc <gnpoc.txt
