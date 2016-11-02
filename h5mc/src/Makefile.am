STD = -fcheck=all -ggdb

LDADD = -lcxxstats
bin_PROGRAMS = h5query h5rank qry01

if HAVE_HDF5
bin_PROGRAMS += h5new shsl

hdf_LDADD = -lhdf5 -lcxxstats
hdf_SOURCES = common.cc common5.cc

h5new_LDADD = -lhdf5 -lcxxstats
h5new_SOURCES = h5new.cc common.cc common5.cc 

shsl_LDADD = $(hdf_LDADD)
shsl_SOURCES = shsl.cc $(hdf_SOURCES)

endif

h5query_SOURCES = h5query.cc common.cc 


#h5rank_LDADD =  -lcxxstats
h5rank_SOURCES = h5rank.cc common.cc

qry01_SOURCES = qry01.cc common.cc


