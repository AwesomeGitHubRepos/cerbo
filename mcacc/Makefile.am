#AM_CXXFLAGS = $(CXXSTD) -O0 -ggdb $(ISUPO)
AM_CXXFLAGS = -std=c++14
AM_CXXFLAGS += -Wfatal-errors

bin_PROGRAMS = mcacc 
man1_MANS = mcacc.1

YFLAGS = -d

BUILT_SOURCES = scanner.cc parser.cc parser.hh
CLEANFILES = $(BUILT_SOURCES)

#mcacc_LDADD = -lpthread $(LIBSUPO)

mcacc_SOURCES =  \ 
	mcacc.cc \
	parser.yy scanner.ll 

# scanner.cc : scanner.ll parser.hh


# parser.hh : parser.cc
