<a id="top"></a>


## sdc-examples

The Small Device C Compiler is a modern cross compiler supporting  multiple
platforms including the Z80.  

The  compiler is intended to target bare metal development boards and knows 
nothing about target hardware and provides no operating system support.

Each target platform requires a separate runtime to provide an interface to
that environment.

This repository provides a minimal interface to support the CP/M  operating 
system.

It also contains a small collection of example programs.

The compiler will automatically select the correct version of the operating 
system specific routines needed to support the compiler's calling standard.

### Contents

Runtime files:
- `sdc-crt0.s` — Minimal CP/M runtime support.
- `sdc-crt0-args.s` — CP/M runtime with argument parser.
- `sdc-cpm.c` — Provides the operating system support functions.

Examples using sdc-crt0:
- `sdc-hello.c` — Of course the first example is "Hello World".
- `sdc-bdos.c` — Calls BDOS to read from console (can't compile with gcc).
- `sdc-calls.c` — A random number generator showing how to call assembler.
- `sdc-getchar.c` — Uses getchar() to read from the keyboard.
- `sdc-gets.c` — Uses fgets() to read from the keyboard.
- `sdc-julia.c` — ASCII Julia set.
- `sdc-life.c` — Displays successive generations of Conway's game of Life.
- `sdc-list.c` — Linked list example.
- `sdc-mandlebrot.c` — ASCII Mandelbrot set.
- `sdc-maze.c` — ASCII Maze generator.
- `sdc-random.c` — Generate 500,000 random numbers using rand().
- `sdc-static.c` — Checks that we can use static variables.

Examples using sdc-crt0-args:
- `sdc-args.c` — Print command line arguments.
- `sdc-calendar.c` — Display the calendar for the specified month or year.
- `sdc-echo.c` — Echo's command line arguments.

- `sdc-tree.c` — Binary tree example (requires large stack).
- `sdc-snake.c` — A turn based snake game (requires large stack).


The  different parameter passing conventions used by newer versions of sdcc 
are handled transparently which allows programs to be compiled without  any
modifications using either SDCC version 3.8 and 4.2. 

The resulting executable files have been tested using ntvcm and CP/M 2.2.

On 4Mhz Z80 generating the Julia set will take approximately 45 minutes! 

### Known Issues
 
If a program crashes (executing an illegal instruction) or just hangs it is
most likely to be because it has run out of stack space.

### Building the CP/M runtime libraries

The C runtime is written for SDCC's Z80 assembler and is used to define the 
starting address and to call init() (which in turn calls main())..

Compile using:
```
sdasz80 -o sdc-crt0.s
OR
sdasz80 -o sdc-crt0-args.s
```

To be able to interact with the operating system programs will use the CP/M
BDOS interface and system specific functions defined in the OS module.  

Since the code in the C runtime calls init() in this module this allows the 
compiler to automatically use the correct calling convention version.

Compile using:
```
sdcc -mz80 -c sdc-cpm.c
```

### Known Issues
 
The only functions available are `putchar()` and `getchar()` which allows a
program to display it's output on the console and read a character from the
console but nothing else is included  (there is no support for files of any 
type).

I did say this was a minimal runtime environment!

### Compiling a program

Once  the runtime libraries are built you are ready to use them in your own
programs. 

Most programs should compile but the limited runtime support means that you 
can  only interact with the console and you also need to remember that some 
data types won't be the same as on modern 64-bit systems.

To compile a program you need to invoke the compiler telling it not to  use 
the standard runtime library and to link program with both the libraries.

A makefile is provided which will compile all the example programs and link
them  with the default runtime library, or you can specify which runtime to 
use and which programs to compile.  

To minimise the size of each program two versions are provided.

- 'sdc-crt0' supports just console output (128 byte stack).

- `sdc-crt0-args` passes `argc` and `argv[]` to `main()` (512 byte stack).

Programs  that require a specific runtime should be built before those that
do not.

The following commands use make to build two programs including support for 
argument passing using `argc` and `argv[]` and then builds the rest.
```
make clean 

make RUNTIME=sdc-crt0-args sdc-calendar sdc-echo

make
```

The makefile also allows you to produce a HEX file compatible with the CP/M 
load command.
```
make RUNTIME=sdc-crt0-args sdc-calendar.hex
```

Alternatively you can compile a program using:
```
sdcc -mz80 --no-std-crt0 --data-loc 0 sdc-crt0.rel sdc-cpm.rel program.c
```

```
-mz80          Compile for the Z80 processor.
--no-std-crt0  Do not use SDCC's standard startup code. 
--data-loc 0   Locate the data area at the start of the generated image.
```

The  output from the compiler is a HEX file that would normally be uploaded 
to the target system.  However for CP/M we need to convert it to a  program 
file that we can either run using an emulator or copy to a CP/M system.

This step is very similar to the load operation on CP/M itself, however the 
file isn't compatible with CP/M's load utility.

Create an executable using:
```
sdobjcopy -Iihex -Obinary --gap-fill 0 program.ihx program.com
```

```
-Iihex         Input file is an Intel HEX file
-Obinary       Output file type is binary
--gap-fill 0   Fill any gaps with '0's
```

### Compatibility

The  resulting COM files have been tested using ntvcm and both CP/M 2.2 and 
NZCOM on simh. 
