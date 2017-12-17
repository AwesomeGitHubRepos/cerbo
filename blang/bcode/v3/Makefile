.PHONY: clean tests

bcode : bcode.cc
	g++ -std=c++14 -ggdb $< -o $@ 

clean:
	rm -f bcode bin.out

tests:
	./run-tests

