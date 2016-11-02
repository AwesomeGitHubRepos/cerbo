AM_CXXFLAGS = -std=gnu++14 -I../../supo -O0 -ggdb
#AM_LDFLAGS = -L../../supo -lsupo
usual =  -L../../supo -lsupo
STD = -fcheck=all -ggdb

bin_PROGRAMS = h5query h5rank qry01
#bin_SOURCES = common.cc h5query.cc h5rank.cc qry01.cc

if HAVE_HDF5
h5libs = $(usual) -lhdf5
bin_PROGRAMS += h5new shsl
#AM_LDFLAGS += -lhdf5 # probably too general, as it links with the vanilla progs
#hdf_LDADD = -lhdf5 $(bin_LDADD)
#bin_SOURCES += common5.cc h5new.cc shsl.cc

#hdf_LDADD = -lhdf5 $(LDADD)
#hdf_SOURCES = common.cc common5.cc

h5new_LDADD = $(h5libs)
h5new_SOURCES = h5new.cc common.cc common5.cc 

shsl_LDADD = $(h5libs)
shsl_SOURCES = shsl.cc common.cc common5.cc

endif

h5query_LDADD = $(usual)
h5query_SOURCES = h5query.cc common.cc 

h5rank_LDADD = $(usual)
h5rank_SOURCES = h5rank.cc common.cc

qry01_LDADD = $(usual)
qry01_SOURCES = qry01.cc common.cc


