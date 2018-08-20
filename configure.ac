AC_PREREQ([2.69])
AC_INIT([cerbo], [0.7], [alt.mcarter@gmail.com])
: ${CXXFLAGS=""}
AC_CONFIG_SRCDIR([green/green.cc])
#AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])


AC_CHECK_HEADERS([readline/readline.h])
AM_CONDITIONAL([HAVE_READLINE], [test x$ac_cv_header_readline_readline_h == xyes])

#CXXFLAGS="$CXXFLAGS -I${srcdir}/supo"
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_LEX
AC_PROG_YACC

AC_SUBST([CXXSTD], ['-std=gnu++14'])
AC_SUBST([ISUPO],   ['-I$(top_srcdir)/supo'])
AC_SUBST([LIBSUPO], ['$(top_builddir)/supo/libsupo.a'])

# Wrapping it up
AC_CONFIG_FILES([Makefile \
		 supo/Makefile \
		 green/Makefile menu/Makefile text2pdf/Makefile \
		 shlex/Makefile \
		 mcacc/Makefile \
		 stats/Makefile])
AC_OUTPUT
