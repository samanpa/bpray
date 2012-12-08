AC_INIT(bp_ray, 0.2.5)

#to get the host call early or we get warning
AC_CANONICAL_SYSTEM

AC_CONFIG_SRCDIR(src/bp_ray.c)
AM_CONFIG_HEADER(src/config.h)
AM_INIT_AUTOMAKE

AC_PROG_CC

#for lex and yacc we should actually check for flex
AM_PROG_LEX
AC_PROG_YACC

#need to check to see if we are building on mingw and set flags
#     for future stages so that right ld will be chosen
case "$host" in
     *-*-mingw*)
	CC="$CC -mno-cygwin"
	platform_win32=yes
	AC_DEFINE(PLATFORM_WIN32,1,[Platform is Win32])
	;;
     *-*-linux*)
	AC_DEFINE(PLATFORM_LINUX,1,[Platform is linux])
	;;
esac

#libtool
AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL

AC_ARG_ENABLE(simd,
	 [--enable-simd Enables the use of simd on this arch ],
	 [
	 case "x$enableval" in
	      xsse2)
	      	AC_DEFINE(SSE,2,[Using sse2])
		;;
	      xsse)
	      	AC_DEFINE(SSE,1,[Using sse])
		;;
	 esac
	 ]
)

case "$host" in
     *-*-mingw*)
          #we already have a porperly built mingw dll in the lib directory
	  #we don't use AC_CHECK_LIB to find glu. opengl32 (a microsoft supplied library)
	  #  cos they are defined __stdcall and so AC_CHECK_LIB willl fail
	  #  FIX: --enable-stdcall-fixup might fix this
	  LIBS="$LIBS -lgd -lopengl32 -lglu32"
	;;
     *)
     	AC_CHECK_LIB(gd, gdFontGetGiant, [], [
	        AC_MSG_ERROR([Error! You need to have libgd around.])
        ])
     	AC_CHECK_LIB(GL, glMatrixMode, [], [
	        AC_MSG_ERROR([Error! You need to have opengl around.])
        ])
     	AC_CHECK_LIB(GLU, gluOrtho2D, [], [
	        AC_MSG_ERROR([Error! You need to have glu around.])
        ])
	;;
esac

AC_PATH_PROG(PKG_CONFIG, [pkg-config], no)
if test "x$PKG_CONFIG" = "xno"; then
   AC_MSG_ERROR([You need to install pkg-config])
fi

pkg-config --atleast-version=2.8 glib-2.0
if [[ "x$?" != "x0" ]] ; then
      AC_MSG_ERROR([You need to have glib installed.])
fi

AC_CHECK_LIB(pthread, pthread_mutex_init, 
,
	[
		AC_MSG_ERROR([You need to have pthreads installed.])
	]
)

AC_ARG_ENABLE(more-warnings,
[  --enable-more-warnings  Maximum compiler warnings],
set_more_warnings="$enableval",[
    set_more_warnings=no
])

CFLAGS=$(pkg-config  --cflags glib-2.0)
LDFLAGS="$(pkg-config --libs glib-2.0) $LDFLAGS"

AC_MSG_CHECKING(for more warnings, including -Werror)
if test "$GCC" = "yes" -a "$set_more_warnings" != "no"; then
    AC_MSG_RESULT(yes)
    CFLAGS="\
    -Wall \
    -Wchar-subscripts -Wmissing-declarations -Wmissing-prototypes \
    -Wnested-externs -Wpointer-arith \
    -Wcast-align -Wsign-compare \
    -std=c1x \
    $CFLAGS" 

    for option in -Wno-strict-aliasing -Wno-sign-compare; do
        SAVE_CFLAGS="$CFLAGS"
        CFLAGS="$CFLAGS $option"
        AC_MSG_CHECKING([whether gcc understands $option])
        AC_TRY_COMPILE([], [],
            has_option=yes,
            has_option=no,)
        if test $has_option = no; then
            CFLAGS="$SAVE_CFLAGS"
        fi
        AC_MSG_RESULT($has_option)
        unset has_option
        unset SAVE_CFLAGS
    done
    unset option
else
    AC_MSG_RESULT(no)
fi


AC_OUTPUT([
Makefile src/Makefile doc/Makefile tests/Makefile
])

echo "
configuration result
      LDFLAGS   : $LDFLAGS
      CFLAGS    : $CFLAGS
      LIBS      : $LIBS
"
