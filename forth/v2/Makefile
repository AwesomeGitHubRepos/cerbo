CFAGS = -ggdb -fsanitize-address
CC = g++
forth : forth.cpp
	$(CC) $(CFLAGS) $^ -o $@ -lasan

clean:
	rm -f forth out

check :
	rm -f out
	./run-tests > out
	@echo "Diffs>"
	@diff expected out

