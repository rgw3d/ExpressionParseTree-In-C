#!/bin/bash

echo 'Preparing the build system...'

if [ ! -d m4 ]; then
	mkdir m4;
fi

if command -v autoreconf >/dev/null 2>&1; then
	autoreconf -vi

	echo ' '
	echo 'The build system is now prepared. To build here, run:'
	echo '  ./configure'
	echo '  make'
	echo 'To debug the program, run:'
	echo '  ./configure --enable-debug'
	echo '  make'
else
	echo 'Error: autoreconf not found.'
fi
