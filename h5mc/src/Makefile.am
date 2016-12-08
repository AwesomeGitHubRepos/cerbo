#AM_CXXFLAGS = -std=gnu++14 -I../.. -O0 -ggdb
AM_CXXFLAGS = $(CXXSTD) $(ISUPO)  -O0 -ggdb
#AM_LDFLAGS = -L../../supo -lsupo
usual =  $(LIBSUPO)
STD = -fcheck=all -ggdb

bin_PROGRAMS = h5query h5rank h5sprep qry01
#bin_SOURCES = common.cc h5query.cc h5rank.cc qry01.cc

if HAVE_FORTRAN
bin_PROGRAMS = hi52w
hi52w_SOURCES = hi52w.f90
#bin_PROGRAMS += spikes
#spikes_SOURCES =  spikes.f90
endif


if HAVE_HDF5
h5libs = $(usual) -lhdf5
#h5libs = $(LIBSUPO) -lhdf5
bin_PROGRAMS += h5new shsl
h5new_LDADD = $(h5libs)
h5new_SOURCES = h5new.cc common.cc common5.cc 

shsl_LDADD = $(h5libs)
shsl_SOURCES = shsl.cc common.cc common5.cc
endif

h5sprep_LDADD = $(usual)
h5sprep_SOURCES = h5sprep.cc common.cc

h5query_LDADD = $(usual)
h5query_SOURCES = h5query.cc common.cc 

h5rank_LDADD = $(usual)
h5rank_SOURCES = h5rank.cc common.cc

qry01_LDADD = $(usual)
qry01_SOURCES = qry01.cc common.cc


