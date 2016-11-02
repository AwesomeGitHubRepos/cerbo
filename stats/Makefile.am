AM_CXXFLAGS = -std=gnu++14 -I../supo
bin_PROGRAMS = stats

stats_LDADD = -L../supo -lsupo
if HAVE_READLINE
stats_LDADD += -lreadline
endif

stats_SOURCES = main.cc parse.cc tests.cc
