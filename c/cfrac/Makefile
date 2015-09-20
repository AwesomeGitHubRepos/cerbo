.PHONY : clean install

clean : 
	rm -f cfrac

install : cfrac
	cp cfrac /usr/local/bin

cfrac : cfrac.c
	gcc cfrac.c -lm -o cfrac
