#define DELAY_BY_REFERENCE 1

#include <dos.h>

unsigned long delay_reference = 0L;

#if DELAY_BY_REFERENCE

/* Michelle's TSM     8088/4.7s7 =  2292 */
/* Doug's Goupil Golf 386SX/16  =  9303 */
/* Geoff's            386SX/20  = 13320 */
/* PS2 Model 70       386SX/20	= 13757 */
/* Jonnie's Intel     386/25    = 21286 */
/* Jonnie's laptop    386/33    = 31186 */
/* PS2 Model 95       486/20	= 48953 */
/* Compaq DeskPro     486/33	= 65316 */

#pragma optimize("",off)

static void init_delay (void)
{
   unsigned long _far *pTicks;
   unsigned long StartTicks;
   unsigned long i = 0L;

   FP_SEG(pTicks) = 0;
   FP_OFF(pTicks) = 0x046C;

   StartTicks = *pTicks;
   while (*pTicks == StartTicks); /* wait for new tick */

   StartTicks = *pTicks+1;
   while (*pTicks != StartTicks) i++;

   delay_reference = i;
}

void delay (unsigned int Ticks)
{
   unsigned long _far *pi;
   unsigned long i = 0L;
   unsigned int j;
   unsigned long mycount = delay_reference;

   if (delay_reference == 0L) init_delay();

   pi = &i;

   for (j=0;j<Ticks;j++)
   {
      i = 0L;
      while (*pi != mycount) i++;
   }
}

#pragma optimize("",on)  /* restore previous values */

#else

void delay(unsigned int Ticks)
/*
旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
� Delay a specified number of ticks ( NB 18.2 ticks/sec )  �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
*/
{
   unsigned long _far *pTicks;
   unsigned long SaveTicks;

   FP_SEG(pTicks) = 0;
   FP_OFF(pTicks) = 0x046C;

   SaveTicks = *pTicks;
   while (*pTicks < SaveTicks + (unsigned long)Ticks)
      if (*pTicks < SaveTicks) break;
}

#endif
