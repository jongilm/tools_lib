#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <direct.h>

#include <tools.h>

int get_new_colour(char *heading, int oldcolour)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int done;
   int ch;
   int i,j;
   int current;
   char buf[11];
   window_t win;

   current = oldcolour;

   win.scrnbufptr    = NULL;
   win.x1            = 13;
   win.y1            = 3;
   win.x2            = 63;
   win.y2            = 22;
   win.window_colour = 0x17;
   win.box_colour    = 0x1F;
   win.box_type      = 1;
   win.shadow_colour = 0x08;
   win.shadow_type   = 1;
   window_up(&win);
   _put_string(win.x1+2,win.y1,heading);

   for (i=0x00;i<=0x0F;i++)
      for (j=0x00;j<=0x0F;j++)
      {
         sprintf(buf,"%2.2X",i*0x10+j);
         _put_text(win.x1+2+j*3,win.y1+2+i,i*0x10+j,buf);
      }

   done = 0;

   while (!done)
   {
      if (current > 0xFF) current = 0xFF;
      if (current < 0x00) current = 0x00;

      _put_text(win.x1+1+(current%0x10)*3  ,win.y1+2+(current/0x10),0x1F,"\x10");
      _put_text(win.x1+1+(current%0x10)*3+3,win.y1+2+(current/0x10),0x1F,"\x11");
       ch = toupper(getch());
       if (!ch) ch = -getch();
      _put_text(win.x1+1+(current%0x10)*3  ,win.y1+2+(current/0x10),0x17," ");
      _put_text(win.x1+1+(current%0x10)*3+3,win.y1+2+(current/0x10),0x17," ");

      switch(ch)
      {
         case PGUP   : current &= 0x0F;                      break;
         case PGDN   : current |= 0xF0;                      break;
         case HOME   : current &= 0xF0;                      break;
         case END    : current |= 0x0F;                      break;
         case UP     : if ((current/0x10) >= 0x01) current -= 0x10; break;
         case DOWN   : if ((current/0x10) <  0x0F) current += 0x10; break;
         case LEFT   : if ((current%0x10) >= 0x01) current -= 0x01; break;
         case RIGHT  : if ((current%0x10) <  0x0F) current += 0x01; break;
         case ESCAPE : done = 1;                             break;
         case ENTER  : done = 2;                             break;
      }
   }

   window_dn(&win);

   if (done == 2)
      return(current);
   else
      return(NO_COLOUR_SELECTED);
}
