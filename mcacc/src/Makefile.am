AM_CXXFLAGS = -std=gnu++14 -ggdb -I$(top_srcdir)
bin_PROGRAMS = mcacc 
man1_MANS = mcacc.1
#mcacc_LDADD = -lpthread -L../.. -lsupo
mcacc_LDADD = -lpthread $(top_builddir)/libsupo.la
mcacc_SOURCES = args.cc \
		cgt.cc common.cc \
		dec.cc \
		epics.cc etb.cc etran.cc etrans-aug.cc \
		gaap.cc \
		inputs.cc \
		mcacc.cc \
		nacc.cc \
		oven.cc \
		posts.cc \
		show.cc stend.cc \
		tests.cc types.cc \
		wiegley.cc \
		yahoo.cc yproc.cc
