#include <conio.h> /* inp & outp */
#include <tools.h>

void beep(unsigned int freq,unsigned int length)
/*
����������������������������������������������������������������������������Ŀ
�                                 beep()                                     �
�                                                                            �
�      This function beeps according to the following arguments.             �
�                                                                            �
�   Argument list:    unsigned int freq    The frequency in Hz.              �
�                     unsigned int length  The duration in milliseconds      �
�                                                                            �
�   Return value:     void                                                   �
�                                                                            �
������������������������������������������������������������������������������
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
����������������������������������������������������������Ŀ
� A cute little beep                                       �
������������������������������������������������������������
*/
{
   beep(1000,100);
   delay(2);
}