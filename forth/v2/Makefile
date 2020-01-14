forth : forth.c
	#gcc -ggdb -fsanitize=address $^ -o $@ -lasan
	gcc -ggdb $^ -o $@ 

clean:
	rm -f forth out

check :
	rm -f out
	./run-tests > out
	@echo "Diffs>"
	@diff expected out

