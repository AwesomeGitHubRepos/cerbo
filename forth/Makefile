forth: forth.c
	#gcc -ggdb -fsanitize=address $^ -o $@ -lasan
	gcc -ggdb $^ -o $@

clean:
	rm -f forth
