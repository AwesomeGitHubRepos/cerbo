# check for existence of libefence
ifeq ($(shell (ldconfig -p | grep -q libefence) && echo -n yes),yes)
#EFENCE = -lefence
endif

# The pie thing seems to be due to some kind of 
# kernel/asan problem on Arch Linux 23-Aug-2017. The no-pie does a workaround
ASAN = -no-pie -fsanitize=address

CHECKER =  $(ASAN)

.PHONY: all clean install

all : blang

clean :
	rm -f blang

blang : blang.cc
	g++ -Wfatal-errors -std=c++17 -ggdb -O0 $(CHECKER) $< -o $@

install :
	install blang ${HOME}/.local/bin
