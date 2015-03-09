ExpressionParseTree-In-C
========================

Same goal as ExpressionParseTreeV2, but I'm writing it in C now. Pointers are my friend

## Building ##
To build this program run the following commands:
```sh
git clone https://github.com/rgw3d/ExpressionParseTree-In-C.git
cd ExpressionParseTree-In-C
make
./main.exe
```

It's currently outputting to `main.exe` because I'm lazy. Currently requires `gcc 4.6+`. To get it to work with other C11 compatible compilers (`clang 3.1+`, etc), edit the makefile.


## Debugging ##
The makefile currently compiles the program with debug symbols. To disable this, remove the `-g` flag in the makefile.

To debug the program run:
```sh
gdb main.exe
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
