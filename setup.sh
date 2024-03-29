#!/bin/sh

if test -z $1; then
    echo no computer specified
    exit;
fi

autogen_CMDLINE="$autogen_CMDLINE --prefix=$HOME"
CFLAGS="-I$HOME/include -march=native "
simd_ISA="sse2"
error=""

for param in $@; do
    case $param in
	windows)
	    autogen_CMDLINE="$autogen_CMDLINE --build=i686-pc-mingw32 "
	    #we add  -lgdi32 -lcomdlg32 which are added when gcc is run with the
	    #   -mwindows parameter. We don't use -mwindows flag because we want to build
	    #   a console application
	    export LDFLAGS="-L`pwd`/lib/i686-pc-mingw32  -lgdi32 -lcomdlg32"
	    ;;
	workpc)
	    export PKG_CONFIG_PATH="$HOME/lib/pkgconfig"
	    ;;
        compute)
            export LDFLAGS="-L$HOME/lib/"
            ;;
	genesis)
            export LDFLAGS="-L$HOME/lib/"
            ;;
	*)
	    ;;
    esac
done

if test -n "$error"; then
    echo ERROR: $error
    exit;
fi

case $simd_ISA in
    sse)
	./autogen.sh $autogen_CMDLINE --enable-simd=sse
        ;;
    sse2)
	./autogen.sh $autogen_CMDLINE --enable-simd=sse2
	;;
    *)
	./autogen.sh $autogen_CMDLINE
	;;
esac;

