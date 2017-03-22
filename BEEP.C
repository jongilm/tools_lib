#include <conio.h> /* inp & outp */
#include <tools.h>

void beep(unsigned int freq,unsigned int length)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 beep()                                     ³
³                                                                            ³
³      This function beeps according to the following arguments.             ³
³                                                                            ³
³   Argument list:    unsigned int freq    The frequency in Hz.              ³
³                     unsigned int length  The duration in milliseconds      ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   unsigned int         oldport,
                        newport,
                        count,
                        locount,
			hicount;

   count = (unsigned)(1193280/freq);     /* main clock frequency/our freq */
   locount = count % 256;                /* lo order byte */
   hicount = count / 256;                /* hi order byte */

   outp(0x43,0xb6);                      /* get timer ready */
   outp(0x42,(unsigned char)locount);    /* load our frequency */
   outp(0x42,(unsigned char)hicount);    /* load our frequency */

   oldport = inp(0x61);                  /* store the old value */
   newport = (oldport|0x03);             /* turn on bits 0 & 1  */
   outp(0x61,newport);                   /* send it - ie turn on speaker */

   delay(length/54);

   outp(0x61,oldport);                   /* turn speaker off */
   return;
}

void pip(void)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ A cute little beep                                       ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   beep(1000,100);
   delay(2);
}
