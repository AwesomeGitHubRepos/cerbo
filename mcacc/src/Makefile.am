#AM_CXXFLAGS = -std=gnu++14 -ggdb -I$(top_srcdir)
AM_CXXFLAGS = $(CXXSTD) -ggdb $(ISUPO)
bin_PROGRAMS = mcacc 
man1_MANS = mcacc.1
#mcacc_LDADD = -lpthread -L../.. -lsupo
#mcacc_LDADD = -lpthread $(top_builddir)/plibsupo.la
mcacc_LDADD = -lpthread $(LIBSUPO)
mcacc_SOURCES = args.cc \
		cgt.cc common.cc \
		dec.cc \
		epics.cc etb.cc etrans-aug.cc \
		gaap.cc \
		inputs.cc \
		mcacc.cc \
		nacc.cc \
		oven.cc \
		posts.cc \
		show.cc stend.cc \
		tests.cc types.cc \
		uprice.cc \
		wiegley.cc \
		yahoo.cc yproc.cc
