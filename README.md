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
aclocal
autoconf
automake -a
./configure
make
```

Outputs to `exptr`. Requires a C99 compatible C compiler.


## Debugging ##
To debug the program run the following commands:
```sh
./configure --enable-debug
make
```

For more information on `gdb`, read the [official documentation](http://www.gnu.org/software/gdb/documentation/).

## Implementation State ##
- [x] Lexer
- [x] Parser gen
- [x] Parser
- [ ] Verify/test parser output
- [ ] Other stuff I haven't thought of

## Dynamic memory allocation and deallocation ##
Since I'm extremely lazy, I'm just keeping track of memory allocated and deallocated with the functions in `memwrapper.h` until something bad happens. 
