cmake_minimum_required(VERSION 3.3.2)
project(maff)
enable_language (Fortran)
add_executable(maff mafftest.f90 maff.f90)

find_package(Doxygen)
if(DOXYGEN_FOUND)
add_custom_target(doc
doxygen 
COMMENT "Generating doxygen docs" VERBATIM
)
endif(DOXYGEN_FOUND)
