FC=gfortran
COPTS=-g -fcheck=all -fbacktrace

.PHONY : all clean

all : maff.mod test
	echo made all

clean :
	rm -f *~ *.o *.mod *.exe examples *.stackdump

%.o %.mod : %.f90
	$(FC) $(COPTS) -c -o $(*F).o $<

ex01 : ex01.f90
	gfortran -o ex01 ex01.f90 

#ex02.o : ex02.f90
#	gfortran -c ex02.f90

ex02 : ex02.f90 maff.f90
	gfortran -o ex02 ex02.f90 maff.f90

ex03 : ex03.f90 maff.f90 maff.mod
	gfortran $(COPTS) -o ex03 ex03.f90 maff.f90

hdf5ex: hdf5ex.f90
	h5fc $(COPTS) $< -o $@

y2nc : y2nc.f90
	gfortran $< -o $@ -I/usr/include  -lnetcdff

quant90 : quant90.f90 quant.mod maff.mod
	$(FC) $(COPTS) $<  quant.o maff.o -o $@

maff90 : maff90.o maff.mod
	$(FC) $(COPTS) $+ -o $@

