/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                scr.c                                       ³
³                                                                            ³
³      This file contains the various functions which make up the screen     ³
³   module of ctools.lib                                                     ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <malloc.h>
#include <dos.h>
#include <string.h>
#include <time.h>
#include <sys\timeb.h>

#include <tools.h>


static char boxch[11][13] =
	/*           0   1   2   3   4   5   6   7   8   9   0   1   2  */
   {  /* 0 */ {' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '},
      /* 1 */ {'Ú','Ä','Â','¿','³','´','Ù','Ä','Á','À','³','Ã','Å'},
      /* 2 */ {'É','Í','Ë','»','º','¹','¼','Í','Ê','È','º','Ì','Î'},
      /* 3 */ {'Õ','Í','Ñ','¸','³','µ','¾','Í','Ï','Ô','³','Æ','Ø'},
      /* 4 */ {'Ö','Ä','Ò','·','º','¶','½','Ä','Ğ','Ó','º','Ç','×'},
      /* 5 */ {'°','°','°','°','°','°','°','°','°','°','°','°','°'},
      /* 6 */ {'±','±','±','±','±','±','±','±','±','±','±','±','±'},
      /* 7 */ {'²','²','²','²','²','²','²','²','²','²','²','²','²'},
      /* 8 */ {'Û','Û','Û','Û','Û','Û','Û','Û','Û','Û','Û','Û','Û'},
      /* 9 */ {'Û','ß','ß','Û','Ş','Ş','Û','Ü','Ü','Û','İ','İ','Û'},
      /*10 */ {'*','*','*','*','*','*','*','*','*','*','*','*','*'}  };

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                getvcols()                                  ³
³                                                                            ³
³      This function determines the number of columns presently being used   ³
³   by the video display via interrupt 0x10, function 0x0F. Most compilers   ³
³   require including the dos.h header file. Several other library routines  ³
³   call this function (e.g., ereol(), erabos(), etc.)                       ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     int      the number of columns currently being used    ³
³                              by the video display (40 or 80).              ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int getvcols(void)
{
   union REGS inregs,outregs;

   inregs.h.ah=0x0F;
   int86(0x10,&inregs,&outregs);

   return(outregs.h.ah);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                               getvmode()                                   ³
³                                                                            ³
³     This function determines the display mode for the video device.        ³
³                                                                            ³
³  Argument list:    void                                                    ³
³                                                                            ³
³  Return value:     int      the display mode, the value of which has       ³
³                             the following meanings:                        ³
³                                                                            ³
³ Mode     Type        Resolution      Adapter(s)      Colors       Address  ³
³ 00h     Text          40 x 25      All but MDA       16 gray       B8000   ³
³ 01h     Text          40 x 25      All but MDA   16 fore/8 back    B8000   ³
³ 02h     Text          80 x 25      All but MDA       16 gray       B8000   ³
³ 03h     Text          80 x 25      All but MDA   16 fore/8 back    B8000   ³
³ 04h   Graphics       320 x 200     All but MDA         4           B8000   ³
³ 05h   Graphics       320 x 200     All but MDA       4 gray        B8000   ³
³ 06h   Graphics       640 x 200     All but MDA         2           B8000   ³
³ 07h     Text          80 x 25        MDA,EGA          b/w          B0000   ³
³ 08h   Graphics       160 x 200        PCjr            16           B0000   ³
³ 09h   Graphics       320 x 200        PCjr            16           B0000   ³
³ 0Ah   Graphics       640 x 200        PCjr             4           B0000   ³
³ 0Bh   Reserved    (internal to EGA)                                        ³
³ 0Ch   Reserved    (internal to EGA)                                        ³
³ 0Dh   Graphics       320 x 200        EGA             16           A0000   ³
³ 0Eh   Graphics       640 x 200        EGA             16           A0000   ³
³ 0Fh   Graphics       640 x 350        EGA             b/w          A0000   ³
³ 10h   Graphics       640 x 350        EGA             16           A0000   ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int getvmode()
{
   union REGS inregs,outregs;

   inregs.h.ah=0x0F;
   int86(0x10,&inregs,&outregs);
   return(outregs.h.al);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                setvmode()                                  ³
³                                                                            ³
³      This function sets the display mode for the video device.             ³
³                                                                            ³
³   Argument list:    int mode   The list of possible values include:        ³
³                                                                            ³
³   0  40x25 BW text, color card         8  160x200 16-color PCjr graphics   ³
³   1  40x25 color text                  9  320x200               "          ³
³   2  80x25 BW text                    10  640x200  4-color      "          ³
³   3  80x25 color                      13  320x200 16-color EGA graphics    ³
³   4  320x200 4-color graphics         14  640x200               "          ³
³   5  320x200 4-color    "             15  640x350 monochrome    "          ³
³   6  640x200 2-color    "             16  640x350 4 or 16-color "          ³
³   7  monochrome text                                                       ³
³                                                                            ³
³   Return value:     void                                                   ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void setvmode(int mode)
{
   union REGS inregs,outregs;

   inregs.h.ah = 0x00; /* Function 0 */
   inregs.h.al = (char)mode;

   int86(0x10, &inregs, &outregs);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                setvgablink()                               ³
³                                                                            ³
³      This function sets the blink/intensity bit for the vga screen.        ³
³                                                                            ³
³   Argument list:    int blink  1=blink, 0=high intensity                   ³
³                                                                            ³
³   Return value:     void                                                   ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void setvgablink(int blink)
{
   union REGS inregs,outregs;

   inregs.h.ah=0x10;
   inregs.h.al=0x03;
   inregs.h.bl=(char)blink;
   int86(0x10,&inregs,&outregs);	/* set blink/intesity bit */
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 getvpage()                                 ³
³                                                                            ³
³      This function gets the active video page.                             ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     int        the currently-active page                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int getvpage(void)
{
   union REGS inregs,outregs;

   inregs.h.ah = 0x0f; /* Function 0x0f */

   int86(0x10, &inregs, &outregs);
   return (outregs.h.bh);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 setvpage()                                 ³
³                                                                            ³
³      This function sets the currently-active video page. Valid page values ³
³   are influenced by the current screen mode [see setvmode()] and the video ³
³   hardware in use. No checking is performed other than to ensure that the  ³
³   page value does not exceed 7.                                            ³
³                                                                            ³
³   Argument list:    int page       the page to be activated                ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
void setvpage(int page)
{
   union REGS inregs,outregs;

   inregs.h.ah = 0x05; /* Function 0x05 */
   inregs.h.al = (char)(page % MAXPAGE);

   int86(0x10, &inregs, &outregs);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 getvdev()                                  ³
³                                                                            ³
³      Function determines whether the video supports color or not. Note     ³
³   that the return value is not affected by the MSDOS "mode" command.       ³
³   The bits 4 and 5 determine video status. If 4 and 5 are on, it is a      ³
³   monochrome card.                                                         ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     int         1   =  monochrome                          ³
³                                 2   =  color                               ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int getvdev(void)
{
   union REGS inregs,outregs;
   int i,device;

   inregs.h.ah=0x12;
   inregs.h.bl=0x10;
   int86(0x10,&inregs,&outregs);
   if (outregs.h.bl<=3)
      device = 3;	/* EGA or VGA */
   else
   {
      int86(0x11, &inregs, &outregs);

      i = outregs.x.ax & 0x30;

      if (i > 0x20) device = 1; /* Monochrome */
      else          device = 2; /* Color */
   }

   return(device);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 getvseg()                                  ³
³                                                                            ³
³      Function determines screen buffer segment                             ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     unsigned int                                           ³
³                                                                            ³
³ Mode     Type        Resolution      Adapter(s)      Colors       Address  ³
³ 00h     Text          40 x 25      All but MDA       16 gray       B8000   ³
³ 01h     Text          40 x 25      All but MDA   16 fore/8 back    B8000   ³
³ 02h     Text          80 x 25      All but MDA       16 gray       B8000   ³
³ 03h     Text          80 x 25      All but MDA   16 fore/8 back    B8000   ³
³ 04h   Graphics       320 x 200     All but MDA         4           B8000   ³
³ 05h   Graphics       320 x 200     All but MDA       4 gray        B8000   ³
³ 06h   Graphics       640 x 200     All but MDA         2           B8000   ³
³ 07h     Text          80 x 25        MDA,EGA          b/w          B0000   ³
³ 08h   Graphics       160 x 200        PCjr            16           B0000   ³
³ 09h   Graphics       320 x 200        PCjr            16           B0000   ³
³ 0Ah   Graphics       640 x 200        PCjr             4           B0000   ³
³ 0Bh   Reserved    (internal to EGA)                                        ³
³ 0Ch   Reserved    (internal to EGA)                                        ³
³ 0Dh   Graphics       320 x 200        EGA             16           A0000   ³
³ 0Eh   Graphics       640 x 200        EGA             16           A0000   ³
³ 0Fh   Graphics       640 x 350        EGA             b/w          A0000   ³
³ 10h   Graphics       640 x 350        EGA             16           A0000   ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

unsigned int getvseg(void)
{
   int mode;

   mode = getvmode();

   if (mode < 0x07)      return(0xB800);
   else if (mode < 0x0B) return(0xB000);
   else                  return(0xA000);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 getvsnow()                                 ³
³                                                                            ³
³      Function determines if screen is suseptible to snow                   ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     int  0 = snow unlikely                                 ³
³                          1 = snow likely                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int getvsnow(void)
{
   if (getvdev() == 2) return(1);
   else                return(0);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 getvgamono()                               ³
³                                                                            ³
³      Function determines if screen is a vga in mono mode                   ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     int  0 = not vga mono                                  ³
³                          1 = is vga mono                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int getvgamono(void)
{
   union REGS inregs,outregs;

   inregs.h.ah=0x12;
   inregs.h.bl=0x10;
   int86(0x10,&inregs,&outregs);
   if (outregs.h.bh==0x01) return(1);
   else                    return(0);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                gotoxy()                                    ³
³                                                                            ³
³      This function places the cursor at (x, y) as given by the function    ³
³   arguments. NOTE: for speed reasons, valid row and column numbers are     ³
³   NOT checked. getvcols() could be used to check valid columns; rows       ³
³   should not exceed 25. The value 1 is subtracted from row and column      ³
³   so that the upper-left corner of the screen is 1,1 rather than 0,0.      ³
³                                                                            ³
³   Argument list:    int x    the column position for the cursor            ³
³                     int y    the row position for the cursor               ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void gotoxy(int x, int y)
{
   union REGS inregs,outregs;

   inregs.h.ah=0x02;
   inregs.h.bh=(char)getvpage();
   inregs.h.dh=(char)(y-1);
   inregs.h.dl=(char)(x-1);
   int86(0x10,&inregs,&outregs);
   return;
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                wherex()                                    ³
³                                                                            ³
³      This function finds the cursor column position.  The value 1 is       ³
³   subtracted from row column so that the upper-left corner of the screen   ³
³   is 1,1 rather than 0,0.                                                  ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     int      The column position of the cursor             ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int wherex()
{
   union REGS inregs,outregs;

   inregs.h.ah=3;
   inregs.x.bx=0;
   int86(0x10,&inregs,&outregs);
   return(outregs.h.dl+1);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                wherey()                                    ³
³                                                                            ³
³      This function finds the cursor row position.  The value 1 is          ³
³   subtracted from row column so that the upper-left corner of the screen   ³
³   is 1,1 rather than 0,0.                                                  ³
³                                                                            ³
³   Argument list:    void                                                   ³
³                                                                            ³
³   Return value:     int      The row position of the cursor                ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

int wherey()
{
   union REGS inregs,outregs;

   inregs.h.ah=3;
   inregs.x.bx=0;
   int86(0x10,&inregs,&outregs);
   return(outregs.h.dh+1);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                getcursorsize()                             ³
³                                                                            ³
³      Function reads the starting and ending scan lines for the cursor.     ³
³   Normal values are: start = 6, end = 7. The function fills in the two     ³
³   pointers with the appropriate values.                                    ³
³                                                                            ³
³   Argument list:    int *start      the starting scan line for the cursor  ³
³                     int *end        the ending scan line for the cursor    ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void getcursorsize(int *start, int *end)
{
   union REGS inregs, outregs;

   inregs.h.ah = 0x03;				  /* Function 3   */
   inregs.h.bh = (char)getvpage(); /* Current page */
   int86(0x10, &inregs, &outregs);

   *start = (int)outregs.h.ch;
   *end   = (int)outregs.h.cl;
   return;
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                setcursorsize()                             ³
³                                                                            ³
³      Function sets the starting and ending scan lines for the cursor.      ³
³   Normal values are: start = 6, end = 7. The function requires two integers³
³   with the appropriate values.                                             ³
³                                                                            ³
³   Argument list:    int start      the starting scan line for the cursor   ³
³                     int end        the ending scan line for the cursor     ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void setcursorsize(int start, int end)
{
   union REGS inregs, outregs;

   inregs.h.ah = 0x01;
   inregs.h.ch = (unsigned char)(start & 0x001F);
   inregs.h.cl = (unsigned char)(end   & 0x001F);
   int86(0x10, &inregs, &outregs);
   return;
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                getcursorshape()                            ³
³                                                                            ³
³      Function reads the starting and ending scan lines for the cursor.     ³
³   Normal values are: start = 6, end = 7. The function fills in the two     ³
³   pointers with the appropriate values.                                    ³
³                                                                            ³
³   Argument list:    int *start      the starting scan line for the cursor  ³
³                     int *end        the ending scan line for the cursor    ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void getcursorshape(int *start, int *end)
{
   getcursorsize(start,end);
   return;
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                setcursorshape()                            ³
³                                                                            ³
³      This function changes the size of the cursor.                         ³
³                                                                            ³
³   Argument list:    int type    0 = None                                   ³
³                                 1 = Normal                                 ³
³                                 2 = Half block                             ³
³                                 3 = Full block                             ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void setcursorshape(int type)
{
   int device;

   device = getvdev();

   switch(type)
   {
      case 0 :	/* No cursor */
         if      (device==1) setcursorsize(0x0E,0x00);
         else if (device==2) setcursorsize(0x08,0x00);
         else if (device==3) setcursorsize(0x0E,0x00);
         break;
      case 1 :	/* Normal cursor */
         if      (device==1) setcursorsize(0x0B,0x0C);
         else if (device==2) setcursorsize(0x06,0x07);
         else if (device==3) setcursorsize(0x0B,0x0C);
         break;
      case 2 :	/* Half block cursor */
         if      (device==1) setcursorsize(0x07,0x0D);
         else if (device==2) setcursorsize(0x04,0x07);
         else if (device==3) setcursorsize(0x07,0x0D);
         break;
      case 3 :	/* Full block cursor */
         if      (device==1) setcursorsize(0x00,0x0D);
         else if (device==2) setcursorsize(0x00,0x07);
         else if (device==3) setcursorsize(0x00,0x0D);
         break;
   }

   return;
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                               scroll()                                     ³
³                                                                            ³
³     This function scrolls the screen via a call to interrupt 0x10. The     ³
³  value of the variable named function may either be 0x07 for an upward     ³
³  scroll, or 0x06 for a scroll in the downward direction. The arguments     ³
³  determines how many columns are presently being used on the screen.       ³
³                                                                            ³
³  Argument list:    int x          the column position for the window       ³
³                    int y          the row position for the window          ³
³                    int dx         the width of the window                  ³
³                    int dy         the depth of the window                  ³
³                    int num        the number of lines to scroll within     ³
³                                   the window (all are scrolled if 0)       ³                                                                     ³
³                    int function   0x07 for up scroll, 0x06 for down        ³
³                    int colour     attribute for blanks in window           ³
³                                                                            ³
³  Return value:     void                                                    ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void scroll(int x1, int y1, int x2, int y2, int num, int function,int colour)
{
   union REGS inregs,outregs;

   inregs.h.ah = (char)function;
   inregs.h.al = (char)num;
   inregs.h.ch = (char)(y1-1); /* Index from (0, 0)    */
   inregs.h.cl = (char)(x1-1);
   inregs.h.dh = (char)(y2-1);
   inregs.h.dl = (char)(x2-1);
   inregs.h.bh = (char)colour; /* attribute byte       */

   int86(0x10, &inregs, &outregs);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                               clrwin()                                     ³
³                                                                            ³
³     This function clears a portion of the screen via a call to scroll.     ³
³                                                                            ³
³  Argument list:    int x          the column position for the window       ³
³                    int y          the row position for the window          ³
³                    int dx         the width of the window                  ³
³                    int dy         the depth of the window                  ³
³                    int colour     attribute for blanks in window           ³
³                                                                            ³
³  Return value:     void                                                    ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void clrwin(int x1, int y1, int x2, int y2, int colour)
{
   union REGS inregs,outregs;

   inregs.h.ah = (char)0x06;
   inregs.h.al = (char)0x00;
   inregs.h.ch = (char)(y1-1); /* Index from (0, 0)    */
   inregs.h.cl = (char)(x1-1);
   inregs.h.dh = (char)(y2-1);
   inregs.h.dl = (char)(x2-1);
   inregs.h.bh = (char)colour; /* attribute byte       */

   int86(0x10, &inregs, &outregs);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                               clreol()                                     ³
³                                                                            ³
³     This function erases to the end of the specified line. The call to     ³
³  getvcols() determines how many columns are presently being used on the    ³
³  screen. Because columns are assumed to begin at 1 (not 0), the starting   ³
³  column value has 1 subtracted from it.                                    ³
³                                                                            ³
³  Argument list:    int x      the column position for the cursor           ³
³                    int y      the row position for the cursor              ³
³                                                                            ³
³  Return value:     void                                                    ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void clreol(int x, int y, int colour)
{
   scroll(x, y, getvcols(), y, 1, SCROLL_DOWN, colour);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                clrl()                                      ³
³                                                                            ³
³      This function erases a specified line.                                ³
³                                                                            ³
³   Argument list:    int y      the row to be erased.                       ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void clrl(int y, int colour)
{
   clreol(1, y, colour);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                               clrbol()                                     ³
³                                                                            ³
³     This function erases to the beginning of the line. The getvcols() call ³
³  determines how many columns are presently being used on the screen.       ³
³                                                                            ³
³  Argument list:    int x      the column position for the cursor           ³
³                    int y      the row position for the cursor              ³
³                                                                            ³
³  Return value:     void                                                    ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void clrbol(int x, int y, int colour)
{
   scroll(1, y, x, y, 1, SCROLL_DOWN, colour);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                clreos()                                    ³
³                                                                            ³
³      This function erases to the end of the screen. The call to getvcols() ³
³   determines how many columns are presently being used on the screen.      ³
³                                                                            ³
³   Argument list:    int x    the column position                           ³
³                     int y    the row position                              ³
³                     int colour                                             ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void clreos(int x, int y, int colour)
{
   clreol(x, y, colour);
   scroll(1, y+1, getvcols(), 25, 25-y, SCROLL_DOWN, colour);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                clrbos()                                    ³
³                                                                            ³
³      This function erases to the beginning of the screen. The call to      ³
³   getvcols() determines how many columns are presently being used on       ³
³   the screen.                                                              ³
³                                                                            ³
³   Argument list:    int x    the column position                           ³
³                     int y    the row position                              ³
³                     int colour                                             ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void clrbos(int x, int y, int colour)
{
   clrbol(x, y, colour);
   scroll(1, 1, getvcols(), y-1, 0, SCROLL_UP, colour);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                clrscr()                                    ³
³                                                                            ³
³      This function erases to the entire screen. The call to                ³
³   getvcols() determines how many columns are presently being used on       ³
³   the screen.                                                              ³
³                                                                            ³
³   Argument list:    int colour                                             ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void clrscr(int colour)
{
   scroll(1, 1, getvcols(), 25, 0, SCROLL_DOWN, colour);
}

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 setborder()                                ³
³                                                                            ³
³      This function sets the border to the color specified as the argument  ³
³   to this function. This function can only be used when mode 4 is active.  ³
³   The symbolic constants are provided in color.h.                          ³
³                                                                            ³
³   Argument list:    int color       the color for the border with values:  ³
³                                                                            ³
³        0   black             8   gray                                      ³
³        1   blue              9   light blue                                ³
³        2   green            10   light green                               ³
³        3   cyan             11   light cyan                                ³
³        4   red              12   light red                                 ³
³        5   magenta          13   light magenta                             ³
³        6   brown            14   yellow                                    ³
³        7   white            15   light white                               ³
³                                                                            ³
³   Return value:     void                                                   ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

void setborder(int colour)
{
   union REGS ireg;

	/*if (getvmode() != 0x04) return;*/  /* Only set border colour in this mode */

   ireg.h.ah = 0x0b;	/* Function 0x0B                       */
   ireg.h.bh = 0;		/* Must be 0 to set border             */
   ireg.h.bl = (char)colour;

   int86(0x10, &ireg, &ireg);
}

void box(int x1,int y1,int x2,int y2,int colour,int type)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 box()                                      ³
³                                                                            ³
³      This function creates a box on the screen according to the following  ³
³   arguments.                                                               ³
³                                                                            ³
³   Argument list:    int x1    the top left column position                 ³
³                     int y1    the top left row position                    ³
³                     int x2    the bottom right column position             ³
³                     int y2    the bottom right row position                ³
³                     int colour                                             ³
³                     int type    where  0 = ' ' no border                   ³
³                                        1 = 'Å' single line                 ³
³                                        2 = 'Î' double line                 ³
³                                        3 = 'Ø' single vert./double horoz.  ³
³                                        4 = '×' double vert./single horoz.  ³
³                                        5 = '°' shade 1                     ³
³                                        6 = '±' shade 2                     ³
³                                        7 = '²' shade 3                     ³
³                                        8 = 'Û' block                       ³
³                                        9 = 'Û' half block                  ³
³                                       10 = '*' asterisk                    ³
³                                                                            ³
³   Return value:     void                                                   ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   int i;
   char line[81];

   line[0] = boxch[type][0];
   for (i=1; i<=x2-x1-1; i++) line[i] = boxch[type][1];
   line[x2-x1] = boxch[type][3];
   line[x2-x1+1] = '\0';
   _put_text(x1,y1,colour,line);

   line[0] = boxch[type][9];
   for (i=1; i<=x2-x1-1; i++) line[i] = boxch[type][7];
   line[x2-x1] = boxch[type][6];
   line[x2-x1+1] = '\0';
   _put_text(x1,y2,colour,line);

   line[1] = '\0';
   for (i=y1+1; i<=y2-1; i++)
   {
      line[0] = boxch[type][10];
      _put_text(x1,i,colour,line);
      line[0] = boxch[type][4];
      _put_text(x2,i,colour,line);
   }
}

int windowstorage(int x1,int y1,int x2,int y2)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ returns amount of storage required for savewindow        ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   return((x2-x1+1+2)*(y2-y1+1+1)*2);
}

char far *pushwindow(int x1,int y1,int x2,int y2)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Save screen                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   char far *scrnbufptr;
   char far *ofs;
   int  y;

   scrnbufptr = _fmalloc(windowstorage(x1,y1,x2,y2));
   if (scrnbufptr)
   {
      ofs = scrnbufptr;
      for (y=y1;y<=y2+1;y++)
      {
         _get_video(x1,y,x2-x1+1+2,ofs);
         ofs     += (x2-x1+1+2)*2;
      }
   }
   return(scrnbufptr);
}

void popwindow(char far *scrnbufptr,int x1,int y1,int x2,int y2)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Restore screen                                           ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   char far *ofs;
   int  y;

   if (scrnbufptr)
   {
      ofs = scrnbufptr;
      for (y=y1;y<=y2;y++)
      {
         _put_video(x1,y,x2-x1+1,ofs);
         ofs     += (x2-x1+1)*2;
      }
      _ffree(scrnbufptr);
   }
}

void pushwindow_noalloc(char far *scrnbufptr, int x1,int y1,int x2,int y2)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Save screen                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   char far *ofs;
   int  y;

   if (scrnbufptr)
   {
      ofs = scrnbufptr;
      for (y=y1;y<=y2;y++)
      {
         _get_video(x1,y,x2-x1+1,ofs);
         ofs     += (x2-x1+1)*2;
      }
   }
}

void popwindow_noalloc(char far *scrnbufptr,int x1,int y1,int x2,int y2)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Restore screen                                           ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   char far *ofs;
   int  y;

   if (scrnbufptr)
   {
      ofs = scrnbufptr;
      for (y=y1;y<=y2;y++)
      {
         _put_video(x1,y,x2-x1+1,ofs);
         ofs     += (x2-x1+1)*2;
      }
   }
}

char far *pushscreen(void)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Save screen                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   char far *scrnptr;
   char far *scrnbufptr;

   FP_SEG(scrnptr) = getvseg();
   FP_OFF(scrnptr) = 0;
   scrnbufptr = _fmalloc(4000);
   if (scrnbufptr) _fmemcpy(scrnbufptr,scrnptr,4000);
   return(scrnbufptr);
}

void popscreen(char far *scrnbufptr)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Restore screen                                           ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   char far *scrnptr;
   if (scrnbufptr != NULL)
   {
      FP_SEG(scrnptr) = getvseg();
      FP_OFF(scrnptr) = 0;
      _fmemcpy(scrnptr,scrnbufptr,4000);
      _ffree(scrnbufptr);
   }
}

void shadow(int x1,int y1,int x2,int y2,int colour,int type)
{
   int i;

   x1--;
   y1--;
   x2--;
   y2--;

   if (type == 1)
   {
      if ((x1 < 0) || (x2 > 77) || (y1 < 0) || (y2 > 23)) return;
      for (i=y1+1;i<=y2+1;i++) _put_attr(x2+1+1,i+1,2,colour);
      _put_attr(x1+2+1,y2+1+1,x2-x1+1,colour);
   }
}

void window_up(window_t *w_ptr)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Make a popup window                                      ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   if (w_ptr->scrnbufptr != NULL) return;
   w_ptr->scrnbufptr = pushwindow(w_ptr->x1,w_ptr->y1,w_ptr->x2,w_ptr->y2);
	/*w_ptr->scrnbufptr = pushwindow(w_ptr->x1,w_ptr->y1,w_ptr->x2+2,w_ptr->y2+1); */
   clrwin(w_ptr->x1,w_ptr->y1,w_ptr->x2,w_ptr->y2,w_ptr->window_colour);
   box(w_ptr->x1,w_ptr->y1,w_ptr->x2,w_ptr->y2,w_ptr->box_colour,w_ptr->box_type);
   shadow(w_ptr->x1,w_ptr->y1,w_ptr->x2,w_ptr->y2,w_ptr->shadow_colour,w_ptr->shadow_type);
}

void window_dn(window_t *w_ptr)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Break a popup window                                     ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   if (w_ptr->scrnbufptr == NULL) return;
   popwindow(w_ptr->scrnbufptr,w_ptr->x1,w_ptr->y1,w_ptr->x2+2,w_ptr->y2+1);
   w_ptr->scrnbufptr = NULL;
}

void window_up_noalloc(window_t *w_ptr)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Make a popup window                                      ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   if (w_ptr->scrnbufptr == NULL) return;
   pushwindow_noalloc(w_ptr->scrnbufptr,w_ptr->x1,w_ptr->y1,w_ptr->x2+2,w_ptr->y2+1);
   clrwin(w_ptr->x1,w_ptr->y1,w_ptr->x2,w_ptr->y2,w_ptr->window_colour);
   box(w_ptr->x1,w_ptr->y1,w_ptr->x2,w_ptr->y2,w_ptr->box_colour,w_ptr->box_type);
   shadow(w_ptr->x1,w_ptr->y1,w_ptr->x2,w_ptr->y2,w_ptr->shadow_colour,w_ptr->shadow_type);
}

void window_dn_noalloc(window_t *w_ptr)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Break a popup window                                     ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   if (w_ptr->scrnbufptr == NULL) return;
   popwindow_noalloc(w_ptr->scrnbufptr,w_ptr->x1,w_ptr->y1,w_ptr->x2+2,w_ptr->y2+1);
}

int ReadField( int x, int y, int colour, int (*validationfunction)(char *field), char *string, char *picture)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                              ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int KeyIn;

   if (x==-1) x = wherex();
   if (y==-1) y = wherey();
   if (colour==-1) colour = 0x70;

   for (;;)
   {
      gotoxy(x,y);
      while (kbhit()) getch();
      KeyIn = edline(string,picture,colour);
      if (KeyIn == ESCAPE) return(FALSE);
      if (KeyIn == ENTER )
      {
         if (validationfunction != NULL)
         {
            if ((validationfunction)(string)) return(TRUE );
         }
         else
            return(TRUE );
      }
   }
}

int PressAnyKey ( int Colour, int ShadowColour )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                              ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int       KeyIn;
   int       oldx;
   int       oldy;

   oldx = wherex();
   oldy = wherey();
   gotoxy(1,26);
   box   (25, 22, 53, 24, Colour, 1);
   shadow(25, 22, 53, 24, ShadowColour, 1);
   _put_field(26, 23, Colour, Colour, 27, " Press any key to continue ");
   pip();

   while (kbhit()) getch();
   KeyIn = getch();
   while (kbhit()) getch();

   gotoxy(oldx,oldy);
   return(KeyIn);
}

unsigned long GetDiskFreeSpace ( int drive )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                              ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   unsigned long space = 0L;
   struct diskfree_t pdisk_params;

   if (!_dos_getdiskfree ((unsigned)drive,&pdisk_params))
      space = (long)pdisk_params.avail_clusters *  pdisk_params.sectors_per_cluster * pdisk_params.bytes_per_sector;

   return (space);
}
