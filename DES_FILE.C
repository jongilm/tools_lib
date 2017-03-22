/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ DES_FILE.C                                                      ³*/
/*³                                                                 ³*/
/*³ DES: Duplicate the NBS Data Encryption Standard in software.    ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/timeb.h>

#include <tools.h>

int encryptfile ( FILE *file1, FILE *file2 );
int decryptfile ( FILE *file1, FILE *file2 );

static void deout   ( char *block,char flag,FILE *file2 );
static char garbage ( void );

static int endesf(char *infile, char *outfile)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   FILE *file1, *file2;
   int error = 0;

   if ((file1 = fopen(infile, "rb")) == NULL)
   {
      /*fprintf(stderr,"Can't read %s.\n", infile);*/
      error = 1;
      goto end;
   }

   if ((file2 = fopen(outfile, "wb")) == NULL)
   {
      /*fprintf(stderr,"Can't write %s.\n", outfile);*/
      error = 2;
      goto end;
   }

   error = encryptfile(file1,file2);

   fclose(file2);
   fclose(file1);

end:
   return(error);
}

static int dedesf(char *infile, char *outfile)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   FILE *file1, *file2;
   int error = 0;

   if ((file1 = fopen(infile, "rb")) == NULL)
   {
      /*fprintf(stderr,"Can't read %s.\n", infile);*/
      error = 1;
      goto end;
   }

   if ((file2 = fopen(outfile, "wb")) == NULL)
   {
      /*fprintf(stderr,"Can't write %s.\n", outfile);*/
      error = 2;
      goto end;
   }

   error = decryptfile(file1,file2);

   fclose(file2);
   fclose(file1);

end:
   return(error);
}

int encryptfile(FILE *file1,FILE *file2)                   /* process the file            */
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   register int m, nsave;
   register char *b;
   int nret;
   int j=0;
   char buf[512];

   while (m = fread(buf, 1, 512, file1))
   {
      if ((nsave = m) < 0)                    /* read error                   */
      {
         /*printf("read error!\n");*/
         return(3);
      }

      for (b=buf; m>0; m -= 8, b += 8)        /* encrypt 1 buffer-full*/
      {                                       /* 8-byte blocks        */
          if (m<8)                         /* don't have a full 64 bits    */
          {
             for (j=0; j<8-m; j++) b[m+j]=garbage(); /* fill block with trash        */
             nsave += 8-m;                 /* complete the block           */
          }
          else
             j=0;                       /* number of nulls in last block*/
          endes(b,b);                     /* don't need diff input, output*/
      }

      if ((nret = fwrite(buf, 1, nsave, file2)) != nsave)
      {
         /*printf("write error!\n");*/
         /*printf("%d bytes to write\n",nsave);*/
         /*printf("%d bytes written\n",nret);*/
         return(4);
      }
   }

   /* have now encrypted/decrypted the whole file;                    */
   /* need to append the byte count for the last block if encrypting. */

   fputc(8 - j, file2);             /* how many good bytes? */
   return(0);
}

int decryptfile(FILE *file1,FILE *file2)                         /* process the file            */
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   register int m, nsave;
   register char *b;
   char buf[512];

   while (m = fread(buf, 1, 512, file1))
   {
      if ((nsave = m) < 0)                    /* read error                   */
      {
         /*printf("read error!\n");*/
         return(3);
      }

      for (b=buf; m>0; m -= 8, b += 8)        /* encrypt/decrypt 1 buffer-full*/
      {                                       /* 8-byte blocks                */
          if (m < 8)
             deout(b, 1, file2);             /* last byte in file: count     */
          else
          {
             dedes(b, b);                    /* decrypt and output block     */
             deout(b, 0, file2);
          }
      }
   }
   return(0);
}

static int outcount = 0;                       /* see when caught up with delay*/

static void deout(char *block,char flag,FILE *file2) /* 1-block delay on output      */
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   static char last[8];            /* previous input block         */
   register int i;

   if (flag)                       /* output the last few bytes    */
   {
      fwrite(last, 1, block[0] & 0xFF, file2);
      return;
   }

   if (outcount++) fwrite(last, 1, 8, file2);  /* seen any blocks before?   */
   for (i = 0; i < 8; i++) last[i] = block[i]; /* copy the block   */
}

static char garbage(void)                            /* generate garbage for filling */
/* This garbage should be as random as possible.  We're using subsequent calls
 * on the timer, but ideally each byte should be uncorrelated.  Preferable
 * would be to call the timer once and use it to initialize a dumb random
 * number generator.
 */
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   struct timeb tp;

   ftime(&tp);                     /* get current time             */
   return((char)(tp.millitm));     /* return time in milliseconds  */
}

