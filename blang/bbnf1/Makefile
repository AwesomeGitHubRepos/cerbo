EXE = bnf

.PHONY : clean
	
$(EXE) : bnf.cc
	g++ --std=c++17 $^ -o $@

clean :
	rm -f $(EXE)
