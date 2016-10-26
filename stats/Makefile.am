bin_PROGRAMS = cxxstats testcxxstats
EXTRA_DIST = tests-env.sh
include_HEADERS = mcstats.h

cxxstats_LDADD = -lreadline -lcxxstats
cxxstats_SOURCES = cxxstats.cc parse.cc

# clue from http://stackoverflow.com/questions/8916425/how-to-create-a-shared-library-so-in-an-automake-script
lib_LTLIBRARIES = libcxxstats.la
libcxxstats_la_SOURCES = mcstats.cc


testcxxstats_LDADD = -lcxxstats
testcxxstats_SOURCES = testcxxstats.cc 

AM_TESTS_ENVIRONMENT = . ./tests-env.sh
AM_TESTS_FD_REDIRECT = 9>&2
TESTS = tests-env.sh
#TESTS = testcxxstats
