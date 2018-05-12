EXE = calc

.PHONY : clean
	
$(EXE) : calc.cc bnf.cc
	g++ --std=c++17 $^ -o $@

clean :
	rm $(EXE)
