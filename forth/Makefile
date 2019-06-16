BINS = forth scanner

all : $(BINS)

forth: forth.c
	gcc -ggdb -fsanitize=address $^ -o $@ -lasan
	#gcc -ggdb $^ -o $@

scanner : scanner.c
	gcc -ggdb $^ -o $@

clean:
	rm -f $(BINS)
