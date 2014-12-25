BUILD = cdf90 test.nc

.PHONY : all clean


all : $(BUILD)

test.nc : cdf.py
	python3 $<

cdf90 : cdf90.f90 
	gfortran $< -I/usr/include  -lnetcdff -o $@

clean :
	rm -f $(BUILD)


	
