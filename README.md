ExpressionParseTree-In-C
========================

Same goal as ExpressionParseTreeV2, but I'm writing it in C now. Pointers are my friend

## Installing ##
To install this program run the following commands:
```sh
./configure
make
make install
```

## Building ##
To build this program from the repo run the following commands:
```sh
git clone https://github.com/rgw3d/ExpressionParseTree-In-C.git
cd ExpressionParseTree-In-C
./autogen.sh
./configure
make
```

Outputs to `src/exptr` or `srx/exptr.exe` on windows. Running `make install` will move the compiled binaries to `$(PREFIX)/bin/` (I think). You can change the prefix by setting the `--prefix=DIR` flag on the configure script. To view the default installation directories run `configure --help`.

To delete the compiled `.o` object files and binaries, run `make clean`.

To change where the program is built, simply run the `configure` script from another folder. For example:
```sh
cd build
../configure
make
```

If any changes were made to `configure.ac` or any `makefile.am`, you need to run `./autogen.sh` before building the program again.

## Debugging ##
To debug the program run the following commands:
```sh
./configure --enable-debug
make
```

For more information on `gdb`, read the [official documentation](http://www.gnu.org/software/gdb/documentation/).

## Documentation ##
To generate documentation from comments in the source code, run the following command:
```sh
doxygen ExpressionParseTree-In-C.doxyfile
```

Requires [doxygen](http://www.stack.nl/~dimitri/doxygen/index.html). The generate documentation can be found in the folder `doc/`.

## Generating parse tree graphs ##
The `parserGenerateDotFile` will generate a `parsetree.dot` file in the working directory. Install a program that can process `.dot` files like [graphviz](http://graphviz.org/) and run the command:
```sh
dot -Tpng -O parsetree.dot
```

Or if you want to get a `.svg` file instead of a `.png`, run the command:
```sh
dot -Tsvg -O parsetree.dot
```

## Implementation state ##
- [x] Lexer
- [x] Parser gen
- [x] Parser
- [x] Verify/test parser output (I think it's right :P)
- [x] Generate dot files for parser output
- [x] Document code
- [ ] Other stuff I haven't thought of

## Dynamic memory allocation and deallocation ##
Since I'm extremely lazy, I'm just keeping track of memory allocated and deallocated with the functions in `memwrapper.h` until something bad happens. This is a quick way for me to remember to free allocated memory.
