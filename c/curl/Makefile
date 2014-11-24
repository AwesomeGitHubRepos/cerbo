.PHONY : all

all : curly socky
 
curly : curly.c
	gcc -ggdb -o curly curly.c -lcurl -lpthread

socky : socky.c
	gcc -ggdb -o socky socky.c
clean:
	rm -f *.exe cruly socky

leaks :
	valgrind --leak-check=yes --show-leak-kinds=all ./curly
