#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([ExpressionParseTree In C], [1.0], [https://github.com/rgw3d/ExpressionParseTree-In-C])

ORIG_CFLAGS=$CFLAGS

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([include/config.h])

AM_INIT_AUTOMAKE

# Debugging support. 
AC_MSG_CHECKING([whether to build with debug information])
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
		[enable debug data generation (default=no)])],
	[debugEnabled="$enableval"],
	[debugEnabled=no])
AC_MSG_RESULT([$debugEnabled])

if test x"$debugEnabled" = x"yes"; then
	AC_DEFINE([DEBUG],[],[Debug Mode])
	AM_CFLAGS="$AM_CFLAGS -g -Wall -O0"

	# Reset compiler flags
	CFLAGS=$ORIG_CFLAGS
else
	AC_DEFINE([NDEBUG],[],[No-debug Mode])
	AM_CFLAGS="$AM_CFLAGS -O3"
fi

AC_SUBST([AM_CFLAGS])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C99

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h stddef.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([makefile src/makefile])
AC_OUTPUT
