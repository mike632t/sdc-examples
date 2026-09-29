

### Comparison of old and new compiler

```
#include <stdio.h>

void main()
{
   int i_count;
   
   for (i_count = 0; i_count <= 9; i_count++)
   {
      puts("Hello there !!");
   }
}
```

SDCC 3.8.0

```
A>load hello

FIRST ADDRESS 0100
LAST  ADDRESS 0312
BYTES READ    00E5
RECORDS WRITTEN 05
```

```
Z80  cycles: 66,407
```
SDCC 4.2.0

```
A>load hello

FIRST ADDRESS 0100
LAST  ADDRESS 02E8
BYTES READ    00BB
RECORDS WRITTEN 04
```

```
Z80  cycles: 34,547
```
In  this case the program produced by the new compiler is 42 bytes  shorter 
and almost twice as fast!

The difference becomes even greater when 

```
#include "stdio.h"         /* Provides printf() for printing output */
#include "stdlib.h"        /* Provides rand() */

#define  MAX_ROWS    50000 

int main() 
{
   unsigned int i_count;
   int i_counter;

   for( i_count = 1; i_count <= MAX_ROWS; i_count++)  
   {
      for( i_counter = 0; i_counter < 10; i_counter++)
         printf("%6d\t", rand());  /* rand() depends on a static variable to store the seed */
      printf("\n");
   }
   return 0;
}
```

```
A>load random

FIRST ADDRESS 0100
LAST  ADDRESS 0FE0
BYTES READ    0DAF
RECORDS WRITTEN 1E
```

```
Z80  cycles: 69,185,479,792
```

```
A>load random

FIRST ADDRESS 0100
LAST  ADDRESS 0EE9
BYTES READ    0CB8
RECORDS WRITTEN 1C
```

```
Z80  cycles: 24,717,667,069
```

This time the new code is 247 bytes shorter and almost three times quicker!



### SDCC Calling Convention


Prior  to version 4.1.2 arguments were pushed onto the stack before calling 
any function and 16-bit values were returned in HL and 8-bit values in A.

Newer  versions of the compiler pass use the registers where possible  with 
16-bit values being returned in DE and 8-bit values returned in A.
 
| Function Signature                            | Parameters         | Register usage  | Description                                   |
| :---                                          | :---               | :---            | :---                                          |
| void fn(uint8_t a)                            | 1 byte             | A               | Byte in A                                     |
| void fn(uint16_t a)                           | 1 word             | HL              | Word in HL                                    |
| void fn(uint8_t a, uint8_t b)                 | 2 bytes            | A, L            | First byte in A, second in L                  |
| void fn(uint16_t a, uint16_t b)               | 2 words            | HL, DE          | First word in HL, second in DE                |
| void fn(uint8_t a, uint16_t b)                | 1 byte, 1 word     | L, DE           | Byte in L, word in DE                         |
| void fn(uint16_t a, uint8_t b)                | 1 word, 1 byte     | HL, E           | Word in HL, byte in DE (E)                    |
| void fn(uint8_t a, uint8_t b, uint8_t c)      | 3 bytes            | A, L + stack    | First byte in A, second in L, third on stack  |
| void fn(uint8_t a, uint8_t b, uint16_t c)     | 2 bytes + 1 word   | A, L + stack    | First byte in A, second in L, word on stack   |
| void fn(uint16_t a, uint16_t b, uint16_t c)   | 3 words            | HL, DE + stack  | First word in HL, second in DE, third on stack|
| void fn(uint8_t a, b, c, d, e)                | 5 bytes            | A, L + stack    | First byte in A, second in L, rest on stack   |
 
