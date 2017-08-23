# check for existence of libefence
ifeq ($(shell (ldconfig -p | grep -q libefence) && echo -n yes),yes)
#EFENCE = -lefence
endif

ASAN = -fsanitize=address

#CHECKER = $(ASAN)

.PHONY: all clean

all : blang

clean :
	rm -f blang

blang : blang.cc
	g++ -Wfatal-errors -std=gnu++14 -ggdb -O0 $(CHECKER) $< -o $@
