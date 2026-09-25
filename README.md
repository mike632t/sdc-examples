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

Note - Newer versions of the SDCC compiler use a different calling standard 
so the runtime files below wont work with with SDCC 4.2.0 or later.  

### Contents

Runtime files:
- `sdc-crt0.s` — Minimal CP/M runtime support.
- `sdc-cpm.c` — Provides the putchar() function used by printf() etc.
- `sdc-crt0-args.s` — CP/M runtime with argument parser.

Examples:
- `sdc-hello.c` — Of course the first example is "Hello World".
- `sdc-mandlebrot.c` — ASCII Mandelbrot set.
- `sdc-julia.c` — ASCII Julia set.
- `sdc-args.c` — Display command line arguments.

The examples were compiled using SDCC version 3.8 and tested on CP/M 2.2.

On 4Mhz Z80 generating the Julia set will take approximately 45 minutes! 

### Known Issues
 
This version uses the original compiler calling convention.  To compile the 
code  with a sdcc version 4.1.12 or later you need to add `--sdcccall 0` to
`CFLAGS` in the makefile.

### Building the CP/M runtime libraries

The C runtime is written for SDCC's Z80 assembler and is used to define the 
starting address and to call main().

Compile using:
```
sdasz80 -o sdc-crt0.s
OR
sdasz80 -o sdc-crt0-args.s
```

To be able to interact with the operating system programs will use the CP/M
BDOS interface defined in the CP/M library.  

Compile using:
```
sdcc -mz80 -c sdc-cpm.c
```

### Known Issues
 
Currently  the only function available is `putchar()` which allows programs 
to display their output on the console but nothing else is included  (there 
is no way to enter text and no support for files of any type).

I did say this was a minimal runtime environment!

### Compiling a program

Once  the runtime libraries are built you are ready to use them in your own
programs. 

Most programs should compile but the limited runtime support means that you 
can  only interact with the console and you also need to remember that some 
data types won't be the same as on modern 64-bit systems.

To compile a program you need to invoke the compiler telling it not to  use 
the standard runtime library and to link program with both the libraries.

Compile a program using:
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

### Known Issues

Currently neither runtime supports static or global variables.  

### Compatibility

The  resulting COM files have been tested using ntvcm and both CP/M 2.2 and 
NZCOM on simh. 
