AM_CXXFLAGS = -std=gnu++14 -I$(top_srcdir)

bin_PROGRAMS = stats
stats_LDADD = $(top_builddir)/libsupo.la
if HAVE_READLINE
stats_LDADD += -lreadline
endif
stats_SOURCES = main.cc tests.cc

