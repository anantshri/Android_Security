AC_INIT(libcfu, 0.03)

AM_INIT_AUTOMAKE([dist-bzip2])

# AC_CONFIG_HEADERS(config.h)

LIBCFU_CHECK_TYPE(u_int, unsigned int)
AC_SUBST(LIBCFU_TYPE_u_int)

AC_PROG_CC
AC_PROG_RANLIB

dnl Check for pthread support
SAVECFLAGS="$CFLAGS"
SAVELIBS="$LIBS"
CFLAGS="-pthread $CFLAGS"
LIBS="-pthread $LIBS"
AC_CHECK_FUNC(pthread_mutex_init,,
    CFLAGS="$SAVECFLAGS"
    LIBS="$SAVELIBS"
    AC_CHECK_LIB(pthread, pthread_mutex_init,,
        AC_CHECK_LIB(pthreads, pthread_mutex_init,,
            AC_MSG_ERROR([No pthreads support found.]))))

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_HEADER_TIME

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_CHECK_FUNCS([gettimeofday memset strcasecmp strncasecmp])

# AC_CONFIG_AUX_DIR

AC_ARG_ENABLE(debug,
	[  --enable-debug    Turn on debugging],
     	[case "${enableval}" in
       	yes) debug=true ;;
       	no)  debug=false ;;
       	*) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
     	esac],[debug=false])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)


AC_CONFIG_FILES([Makefile
	src/Makefile
	examples/Makefile
	doc/Makefile
	include/cfutypes.h
	include/cfu.h])

AC_OUTPUT
