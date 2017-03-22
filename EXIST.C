
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>

int FileExists ( char *Filename )
{
   unsigned int Error;
   unsigned int Attr;
   unsigned int FilenameSeg;
   unsigned int FilenameOff;
   char _far *p;
   p = Filename;
   FilenameSeg = FP_SEG(p);
   FilenameOff = FP_OFF(p);

   _asm
   {
      mov Error, 0
      mov ah ,43h
      mov al ,0h
      push ds
      push dx
      mov ds ,FilenameSeg
      mov dx ,FilenameOff
      int 21h
      jnc noerror
      mov Error, 1
   noerror:
      mov Attr, CX
   }
   if (Error)
      return 0;

   return 1;
}

/*
int main ( int argc, char *argv[] )
{
   if (FileExists(argv[1]))
      printf("%s does exist\n",argv[1]);
   else
      printf("%s does not exist\n",argv[1]);

   return 0;
}
*/
