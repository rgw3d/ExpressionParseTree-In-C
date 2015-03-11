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

Outputs to `exptr`. Requires a C99 compatible C compiler. If you get an error about missing a `m4` directory, simply run `mkdir m4`. Too lazy to include it in the repo cuz I'm lazy.

To delete the compiled `.o` object files and executable files, run `make clean`.

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

## Implementation State ##
- [x] Lexer
- [x] Parser gen
- [x] Parser
- [ ] Verify/test parser output
- [ ] Generate dot files for parser output
- [ ] Document code
- [ ] Other stuff I haven't thought of

## Dynamic memory allocation and deallocation ##
Since I'm extremely lazy, I'm just keeping track of memory allocated and deallocated with the functions in `memwrapper.h` until something bad happens. 
