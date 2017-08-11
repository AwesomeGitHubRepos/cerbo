AM_CXXFLAGS = -std=gnu++14 $(ISUPO)

bin_PROGRAMS = stats
#stats_LDADD = $(top_builddir)/libsupo.la


stats_LDADD = $(LIBSUPO)

# Quirk in Slackware 14.2 means that we have to link against
# ncurses in order to get readline to work, too.
if HAVE_READLINE
stats_LDADD += -lreadline -lncurses
endif
stats_SOURCES = main.cc tests.cc

