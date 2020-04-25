forth : forth.c
	#gcc -ggdb -fsanitize=address $^ -o $@ -lasan
	gcc -std=c11 -ggdb $^ -o $@ -lm 

clean:
	rm -f forth out

check :
	rm -f out
	./run-tests > out
	@echo "Diffs>"
	@diff expected out

