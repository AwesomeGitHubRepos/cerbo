bcode : bcode.cc
	g++ -std=c++14 -ggdb -fsanitize=address -fno-omit-frame-pointer $< -o $@ -lasan

clean:
	rm -f bcode bin.out

