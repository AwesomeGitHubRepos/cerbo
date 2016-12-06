AM_CXXFLAGS = -std=gnu++14 $(ISUPO)

bin_PROGRAMS = stats
#stats_LDADD = $(top_builddir)/libsupo.la
stats_LDADD = $(LIBSUPO)
if HAVE_READLINE
stats_LDADD += -lreadline
endif
stats_SOURCES = main.cc tests.cc

