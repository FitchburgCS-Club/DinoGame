AC_INIT([dino], [1.0], [compsci@student.fitchburgstate.edu])
AM_INIT_AUTOMAKE([-Wall subdir-objects foreign])
AC_PROG_CC
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_SRCDIR([src])
AC_CONFIG_FILES([
	Makefile
])
AC_SEARCH_LIBS([SDL_Init], [SDL2], [], [AC_MSG_ERROR([Unable to find SDL2])])
AC_CHECK_HEADERS([SDL2/SDL_image.h],[],[AC_MSG_ERROR([Unable to find SDL2_Image])])
AC_ARG_ENABLE([debug],
[  --enable-debug    Turn on debugging],
[case "${enableval}" in
  yes) debug=true ;;
  no)  debug=false ;;
  *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],[debug=false])
AM_CONDITIONAL([DEBUG], [test x$debug = xtrue])
AC_OUTPUT
