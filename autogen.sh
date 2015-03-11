#!/bin/bash

echo 'Preparing the build system...'

if command -v autoreconf >/dev/null 2>&1; then
	autoreconf -vi

	echo 'The build system is now prepared. To build here, run:'
	echo '  ./configure'
	echo '  make'
else
	echo 'Error: autoreconf not found.'
fi
