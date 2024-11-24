<a id="top"></a>


## sdc-examples

A minimal runtime environment to allow SDCC to target systems running CP/M.

It also contains a small collection of example programs.

Note - Newer versions of the SDCC compiler use a different calling standard 
so you need to use `--sdcccall 0` when using these CP/M libraries with SDCC 
4.2.0 or later.  

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

### Building the CP/M runtime libraries

The C runtime is written for SDCC's Z80 assembler and is used to define the 
starting address and to call main().

Compile using:
```
sdasz80 -o sdc-crt0.s
```
To be able to interact with the operating system programs will use the CP/M
BDOS interface defined in the CP/M library.  

Note - This version only allows programs to display text on the console.

Compile using:
```
sdcc -mz80 -c sdc-cpm.c
```

### Compiling a program

Once the runtime libraries are built you are ready to use the compiler.

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
