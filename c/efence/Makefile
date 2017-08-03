all: efence1 efence2

clean: efence1 efence2
	rm -f efence1 efence2

efence1 : efence_test.c
	gcc -g -Wall -lefence $< -o $@

efence2 : efence_test.c
	gcc -g -Wall $< -o $@
