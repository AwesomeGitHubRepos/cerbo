AM_CXXFLAGS = -std=gnu++14

bin_PROGRAMS = stats
stats_LDADD = ../libsupo.la
if HAVE_READLINE
stats_LDADD += -lreadline
endif
stats_SOURCES = main.cc tests.cc

