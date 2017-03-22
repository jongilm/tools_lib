#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <direct.h>

#include <tools.h>

static int PromptColour     = 0x0F;
static int SelectedColour   = 0x07;
static int UnselectedColour = 0x70;

extern fieldtype   *pField;
extern int          numberoffields;

void SetFieldColours ( int Prompt, int Selected, int Unselected )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                  ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   PromptColour     = Prompt;
   SelectedColour   = Selected;
   UnselectedColour = Unselected;
}

void SetFieldInfo( int ndx, int x, int y, char *prompt, int type, char *picture, void *pdata)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   (pField+ndx)->x = x;
   (pField+ndx)->y = y;
   strcpy((pField+ndx)->prompt,prompt);
   (pField+ndx)->type = type;
   strcpy((pField+ndx)->picture,picture);
   (pField+ndx)->pdata = pdata;
}

void DisplayFields(void)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int i;

   for (i=0;i<numberoffields;i++)
   {
      _put_text((pField+i)->x,(pField+i)->y,PromptColour,(pField+i)->prompt);
      if ((pField+i)->type == 'A')
         _put_field ((pField+i)->x+strlen((pField+i)->prompt),
                     (pField+i)->y,
                     UnselectedColour,UnselectedColour,
                     strlen((pField+i)->pdata),
                     (pField+i)->pdata);
      else
      {
         char Str[81];
         itoa(*((int *)(pField+i)->pdata),Str,10);
         _put_field ((pField+i)->x+strlen((pField+i)->prompt),
                     (pField+i)->y,
                     UnselectedColour,UnselectedColour,
                     strlen(Str),
                     Str);
      }
   }
}

int EditField(int ndx)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int last_char;
   switch ((pField+ndx)->type)
   {
      case 'A' : last_char = EditFieldAlpha((pField+ndx)->x,
                                        (pField+ndx)->y,
                                        (pField+ndx)->prompt,
                                        (pField+ndx)->picture,
                                        (pField+ndx)->pdata);
                 break;
      case 'N' : last_char = EditFieldNumeric ((pField+ndx)->x,
                                        (pField+ndx)->y,
                                        (pField+ndx)->prompt,
                                        (pField+ndx)->picture,
                                        (pField+ndx)->pdata);
                 break;
      default  : last_char = 27;
   }
   return(last_char);
}

char *Pic(int ch, int length)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   static char picture[81];
   memset(picture,ch,length);
   picture[length] = '\0';
   return(picture);
}

int EditFieldAlpha(int x, int y, char *prompt, char *picture, char *zstr)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int last_char;
   _put_text(x,y,PromptColour,prompt);
   gotoxy(x+strlen(prompt),y);
   last_char = edline(zstr,picture,SelectedColour);
   _put_field(x+strlen(prompt),y,UnselectedColour,UnselectedColour,strlen(picture),zstr);
   return(last_char);
}

int EditFieldNumeric(int x, int y, char *prompt, char *picture, int *value)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int last_char;
   char zstr[21];

   itoa(*value,zstr,10);

   _put_text(x,y,PromptColour,prompt);
   gotoxy(x+strlen(prompt),y);
   last_char = edline(zstr,picture,SelectedColour);
   _put_field(x+strlen(prompt),y,UnselectedColour,UnselectedColour,strlen(picture),zstr);
   *value = atoi(zstr);

   return(last_char);
}

int EditFieldHex(int x, int y, char *prompt, char *picture, char *value)
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int last_char,tempint;
   char zstr[21];

   tempint = *value;
   sprintf(zstr,"%2.2X",tempint);

   _put_text(x,y,PromptColour,prompt);
   gotoxy(x+strlen(prompt),y);
   last_char = edline(zstr,picture,SelectedColour);
   _put_field(x+strlen(prompt),y,UnselectedColour,UnselectedColour,strlen(picture),zstr);

   sscanf(zstr,"%x",&tempint);
   *value = (char)tempint;

   return(last_char);
}
