AM_CXXFLAGS = -std=gnu++14

AUTOMAKE_OPTIONS = subdir-objects
#AUTOMAKE_OPTIONS = std-options
lib_LTLIBRARIES = libsupo.la
libsupo_la_SOURCES = supo.cc supo_stats.cc

bin_PROGRAMS = stats
#bin_PROGRAMS = menu

#menu_LDADD = libsupo.la
#menu_SOURCES = menu/menu.cc

stats_LDADD = libsupo.la
if HAVE_READLINE
stats_LDADD += -lreadline
endif
stats_SOURCES = stats-src/main.cc stats-src/parse.cc stats-src/tests.cc

SUBDIRS = green text2pdf shlex mcacc/src  h5mc/src h5mc/scripts menu \
	  splut

