AC_INIT(autogen.sh)
AM_INIT_AUTOMAKE(cdargs, 1.35)
AC_PROG_CXX
#AC_STDC_HEADERS

dnl AC_CHECK_CURSES
dnl if test "$has_curses" = "false" ; then
dnl  AC_MSG_ERROR([Fatal error: couldn't find ncurses/curses.])
dnl fi

dnlMP_WITH_CURSES
dnlif test "x$CURSES_LIB" = "x" ; then
dnl  AC_MSG_ERROR([Fatal error: couldn't find ncurses/curses.])
dnlfi
dnlAC_SUBST(CURSES_LIB)

AC_OUTPUT(Makefile src/Makefile )
#AC_OUTPUT(contrib/Makefile doc/Makefile m4/Makefile cdargs.spec)
