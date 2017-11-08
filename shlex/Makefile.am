#AM_CXXFLAGS = -std=gnu++14 -I../../supo
AM_CXXFLAGS = $(CXXSTD) $(ISUPO)
bin_PROGRAMS = shlex  sumby tsv

man1_MANS = shlex.1

shlex_LDADD = $(LIBSUPO)
shlex_SOURCES = shlex.cc

sumby_SOURCES = sumby.cc

tsv_LDADD = $(LIBSUPO)
tsv_SOURCES = tsv.cc
