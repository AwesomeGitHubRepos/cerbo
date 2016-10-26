AM_CXXFLAGS = -std=gnu++14
bin_PROGRAMS = stats

if HAVE_READLINE
stats_LDADD = -lreadline
endif

stats_SOURCES = main.cc parse.cc stats.cc tests.cc
