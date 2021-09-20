The ChipCode Programming Language
=================================

ChipCode is a compiled, bytecode interpreted programming language. The syntax was designed relatively close to C to provide a "C" style vibe to it and also a virtual machine for potability. 

The whole point of this project is to not only create a functioning compiler but to teach how compilers work in the simplest way possible. 

The bytecodes are very similar to Java as it is a stack machine rather than a register machine. In the future I may write a transpiler to convert it to JVM bytecodes. 

This compiler generates bytecodes without any optimization. 

This program is meant for educational purposes only and for fun, please DO NOT use it for serious applications. Your program might crash. 

Click [here](https://github.com/coldchip/chipvm) for the virtual machine.

The virtual machine uses `syscall` for the bytecode to interact with the internals. 