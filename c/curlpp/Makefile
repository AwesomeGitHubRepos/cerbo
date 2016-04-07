AMFLAGS = -std=c++11

.PHONY : all clean

all : curly

clean :
	rm -f *.o curly

curly.o : curly.cpp curly.h
	$(CXX) $(AMFLAGS) -c $<

main.o : main.cpp
	$(CXX) $(AMFLAGS) -c $<

curly : main.o curly.o
	$(CXX) $(AMFLAGS) $^ -o $@ -lpthread -lcurl
