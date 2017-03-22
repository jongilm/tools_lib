
#include <string.h>
#include "dat.h"

char *month_names[12] = { "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP", "OCT", "NOV", "DEC" };

#define T0 1721060L

long dmy_to_day(int day, int month, long year, int julian)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                  ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   static int m[13] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
                 /*       Ja  Fe  Ma  Apr  May  Jun  Jul  Aug  Sep  Oct  Nov  Dec   */
   long rval;

   if( year >= 0L)
   {
      rval = year*365L + year / 4L;
      if (!julian)
         rval += -year / 100L + year / 400L;
   }
   else
   {
      rval = year*365L + (year-3L) / 4L;
      if (!julian)
         rval += - (year-99L) / 100L + (year-399L) / 400L;
   }

   if (julian)
      rval -= 2L;
   if (month<3 && !(year % 4L))
      if ( (year % 100L) || !(year % 400L) || julian)
         rval--;
   rval += (long)m[month - 1] + (long)day;
   return rval+T0;
}

void day_to_dmy( long day, int *d, int *m, long *y, int julian)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                  ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   *y = ( day - T0) / 365L - 3L;
   while( dmy_to_day( 1, 1, *y, julian) < day)
      (*y)++;
   while( dmy_to_day( 1, 1, *y, julian) > day)
      (*y)--;
   for( *m = 1; dmy_to_day(0, (*m)+1, *y, julian) < day; (*m)++);
   *d = (int)( day - dmy_to_day(0, *m, *y, julian));
}


////////////////////// ISLAM DATES ////////////////////////////

#define THIRTY_YEARS 10631L
#define islam_T0 1948086L

statiic char *islam_leap = "aaabbbccdddeeefffgghhhiiijjkkkl";
                 /* 012345678901234567890123456789 */

char *islam_month_name[12] = {
      "Muharram", "Safar", "Rabia I", "Rabia II", "Jumada I", "Jumada II",
      "Rajab", "Shaban", "Ramadan", "Shawwai", "Zu'lkadah", "Zu'lhijah" };

long islam_dmy_to_day(int day, int month, long year)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                  ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   long rval;

   month--;
   rval = (long)(year / 30L) * THIRTY_YEARS;
   year %= 30L;
   rval += year * 354L + (long)(islam_leap[year] - 'a');
   rval += (long)((month / 2) * 59 + (month & 1) * 30 + day - 1);
   return( rval + islam_T0);
}

void islam_day_to_dmy( long day, int *d, int *m, long *y)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                  ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int i;

   *y = ((day - islam_T0) / THIRTY_YEARS) * 30L;
   day = (day - islam_T0) % THIRTY_YEARS;
   for ( i = 1; i < 31; i++)
   {
      if( (int)day < 354 * i + (int)(islam_leap[i] - 'a'))
      {
         i--;
         *y += i;
         day -= (long)i * 354L + (long)(islam_leap[i] - 'a');
         *m = 2 * (int)(day / 59L);
         if( *m == 12)        /* leap day fix */
            *m = 10;
         day -= (long)*m * 59L / 2L;
         if( day >= 30L)
         {
            day -= 30L;
            (*m)++;
         }
         *d = (int)day + 1;
         (*m)++;
         return;
      }
   }
}













/*  Test code

#include <stdio.h>
#include <stdlib.h>
#include "date.h"

void main( int argc, char *argv[])
{
   int d, m;
   long jul_day, y;

   if( argc < 4)
      {
      printf( "DATTEST expects a day,  month,  and year as command line\n");
      printf( "arguments.  For example,  28 Nov 1929 would be run as:\n\n");
      printf( "dattest 28 11 1929\n");
      exit( 0);
      }

   d = atoi( argv[1]);
   m = atoi( argv[2]);
   y = atol( argv[3]);

   jul_day = dmy_to_day( d, m, y, 0);
   printf( "julian day = %ld\n", jul_day);
   day_to_dmy( jul_day, &d, &m, &y, 0);
   printf( "converted back: %d %d %ld (greg calendar)\n", d, m, y);

   day_to_dmy( jul_day, &d, &m, &y, 1);
   printf( "%d %d %ld (julian calendar)\n", d, m, y);
   jul_day = dmy_to_day( d, m, y, 1);
   printf( "converted back: day = %ld\n", jul_day);

   islam_day_to_dmy( jul_day, &d, &m, &y);
   printf( "%d %d %ld (Islamic calendar)\n", d, m, y);
   jul_day = islam_dmy_to_day( d, m, y);
   printf( "converted back: day = %ld\n", jul_day);
}
*/
