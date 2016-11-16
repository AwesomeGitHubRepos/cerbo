AC_PREREQ([2.69])
AC_INIT([cerbo], [0.5], [alt.mcarter@gmail.com])
AC_CONFIG_SRCDIR([green/green.cc])
#AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])

AC_CHECK_HEADERS([hdf5.h])
AM_CONDITIONAL([HAVE_HDF5], [test x$ac_cv_header_hdf5_h == xyes])


AC_CHECK_HEADERS([readline/readline.h])
AM_CONDITIONAL([HAVE_READLINE], [test x$ac_cv_header_readline_readline_h == xyes])

#CXXFLAGS="$CXXFLAGS -I${srcdir}/supo"
AC_PROG_CXX
#AC_PROG_RANLIB
AC_PROG_LIBTOOL

AC_PROG_FC
AM_CONDITIONAL([HAVE_FORTRAN], [test x$FC == xgfortran])


# Wrapping it up
AC_CONFIG_FILES([Makefile green/Makefile menu/Makefile text2pdf/Makefile \
		 shlex/Makefile stats/Makefile mcacc/src/Makefile \
		 h5mc/src/Makefile])
AC_OUTPUT
