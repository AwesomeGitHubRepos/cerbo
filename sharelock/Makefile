.PHONY : clean config cookies main

main:
	@echo Be more specific: make config, cookies

config:
	mkdir -p int/calcs
	mkdir -p int/cards
	mkdir -p int/fetches
	mkdir -p int/misc

cookies: # reduce the size of the cookies file
	fookies | sed -e '/sharelock\|^#/!d' > ~/.fortran/cookies.txt


report01 : report01.c
	gcc -ggdb -o report01 report01.c -lm -lgsl -lgslcblas

clean :
	rm -f report01


