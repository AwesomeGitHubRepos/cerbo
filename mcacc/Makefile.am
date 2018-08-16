AM_CXXFLAGS = $(CXXSTD) -O0 -ggdb $(ISUPO)
AM_CXXFLAGS += -Wfatal-errors
bin_PROGRAMS = mcacc 
man1_MANS = mcacc.1

YFLAGS = -d

BUILT_SOURCES = scanner.cc parser.cc parser.hh
CLEANFILES = $(BUILT_SOURCES)

mcacc_LDADD = -lpthread $(LIBSUPO)
mcacc_SOURCES = args.cc \
		cgt.cc common.cc \
		dec.cc \
		epics.cc etb.cc etran.cc \
		gaap.cc \
		inputs.cc \
		mcacc.cc \
		oven.cc \
		posts.cc \
		show.cc stend.cc \
		tests.cc types.cc \
		uprice.cc \
		wiegley.cc \
		yahoo.cc yproc.cc \
		parser.yy scanner.ll 

# scanner.cc : scanner.ll parser.hh


# parser.hh : parser.cc
