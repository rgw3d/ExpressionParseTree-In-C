ExpressionParseTree-In-C
========================

Same goal as ExpressionParseTreeV2, but I'm writing it in C now. Pointers are my friend

## Building ##
To build this program run the following commands:
```sh
git clone https://github.com/Jinbobo/ExpressionParseTree-In-C.git
cd ExpressionParseTree-In-C
make all
./main.exe
```

It's currently outputting to `main.exe` because I'm lazy. Currently requires `gcc 4.6+`. To get it to work with other C11 compatible compilers (`clang 3.1+`, etc), edit the makefile.

The makefile currently also compiles the program with debug symbols. To disable this, remove the `-g` flag in the makefile.

## Implementation State ##
- [x] Lexer
- [x] Parser gen
- [ ] Parser
- [ ] Other stuff I haven't thought of

## Dynamic memory allocation and deallocation ##
Since I'm extremely lazy, I'm just keeping track of memory allocated and deallocated with the functions in `memorywrapper.h` until something bad happens. 