#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <dos.h>
#include <direct.h>

#include <tools.h>

tMenu::tMenu ( char *Heading, int NumberOfItems )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   _Debugging     = 0;
   _MaxItems      = NumberOfItems;

   _pItem = (MENU_OPTION *)malloc( sizeof(MENU_OPTION) * _MaxItems );
   if (_pItem == NULL)
   {
      _MaxItems = 0;
      return;
   }

   InitialiseData ();
   strcpy ( _Heading, Heading  );
   _NumberOfItems = NumberOfItems;
}

tMenu::~tMenu ()
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   if (_pItem) free (_pItem);
}

void tMenu::InitialiseData ( void )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   int i;

   _Heading[0] = 0;
   strcpy ( _BackgroundCharacter, "±" );
   _BackgroundColour       = 0x07;
   _BorderColour           = 0x0F; /* not used */
   _NormalColour           = 0x07;
   _HighlightColour        = 0x0F;
   _ShadowColour           = 0x08;
   _SelectedItemNumber     = 0;
   _NumberOfItems          = 0;

   if (_pItem == NULL) return;

   for (i=0;i<_MaxItems;i++)
      memset( &_pItem[i], 0, sizeof(MENU_OPTION) );
}

int tMenu::MaxItems ( void )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   return _MaxItems;
}

int tMenu::MenuProcess  ( int InitialItemNumber, int QuitKey, int EditKey )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                          ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   if (_pItem == NULL) return 1;
   _SelectedItemNumber = InitialItemNumber;
#ifdef DEBUG
   if (Debugging)
      Debug ();
#endif

   menu_paint ();
   ExtraPaint();

   return menu_choice ( QuitKey, EditKey );
   // 0 = Item selected
   // 1 = QuitKey
   // 2 = EditKey
}

char tMenu::ExtractHotkey ( char *Description, int RemoveHotkeyFromString )
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
{
   char *p;
   char Hotkey = ' ';

   p = strchr(Description,'&');
   if (p!=NULL)
   {
      Hotkey = *(p+1);
      if (RemoveHotkeyFromString)
      {
         memcpy(p,p+1,strlen(p));
         //_put_string(1,1,Description);
      }
   }
   return Hotkey;
}

void tMenu::menu_paint ( void )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                          ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int x,y,delta_x,delta_y,i,j,hot_pos;
   char *hot_ptr;

   gotoxy(1,26);
   menu_background ();
   menu_header ();

   delta_x = menu_width () + 4;
   delta_y = _NumberOfItems + 4;

   x = (getvcols() - delta_x)/2;
   y = (25 - delta_y)/2;

   clrwin ( x, y, x+delta_x-1, y+delta_y-1, _NormalColour     );
   box    ( x, y, x+delta_x-1, y+delta_y-1, _HighlightColour,3);
   shadow ( x, y, x+delta_x-1, y+delta_y-1, _ShadowColour   ,1);

   j = 0;
   for (i=0;i<_NumberOfItems;i++)
   {
      char Description[81];
      char Hotkey;

      strcpy ( Description, _pItem[i].Description );
      Hotkey = ExtractHotkey ( Description, TRUE );

      hot_ptr = strchr(Description,Hotkey);
      if (!hot_ptr)
         hot_ptr = strchr(Description,toupper(Hotkey));
      if (!hot_ptr)
         hot_ptr = strchr(Description,tolower(Hotkey));
      if (!hot_ptr)
         hot_ptr = Description;

      hot_pos = hot_ptr - Description;

      _put_attr(x+2+hot_pos,y+2+j,1,_HighlightColour);

      _put_string(x+2,y+2+j,Description);
      j++;
   }
}

int tMenu::menu_choice ( int QuitKey, int EditKey )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                          ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int ch,i;
   int x,y,delta_x,delta_y;
   char old_attrs[81];
   char new_attrs[81];

   delta_x = menu_width ();
   delta_y = _NumberOfItems;
   x = (getvcols() - delta_x)/2;
   y = (25 - delta_y)/2;

   for(;;)
   {
      if ((_SelectedItemNumber) < 0)        (_SelectedItemNumber) = delta_y-1;
      if ((_SelectedItemNumber) >= delta_y) (_SelectedItemNumber) = 0;
      if ((delta_y > 0) && (delta_x > 0))
      {
         _get_attrs(x-1,y+(_SelectedItemNumber),delta_x+2,old_attrs);
         for (i=0;i<delta_x+2;i++)
         {
            if ( _NormalColour == 0x70 ) new_attrs[i] = 0x07;
            else                                     new_attrs[i] = 0x70;
         }
         _put_attrs(x-1,y+(_SelectedItemNumber),delta_x+2,new_attrs);
      }

      ch = toupper(getch());
      if (!ch) ch = -getch();

      if ((delta_y > 0) && (delta_x > 0))
      {
         _put_attrs(x-1,y+_SelectedItemNumber,delta_x+2,old_attrs);
      }
#ifdef DEBUG
   if (Debugging)
      if (ch == SHIFT_F10 ) { system("command"); continue; }
#endif
      if (ch == QuitKey) return(1);
      if (ch == EditKey) return(2);

      switch (ch)
      {
         case UP    :
         case LEFT  : _SelectedItemNumber--;
                      break;
         case DOWN  :
         case RIGHT : _SelectedItemNumber++;
                      break;
         case PGUP  :
         case HOME  : _SelectedItemNumber = 0;
                      break;
         case PGDN  :
         case END   : _SelectedItemNumber = delta_y-1;
                      break;
         case ENTER : if (delta_y < 1) break;
                      return(0);
         default    : if (delta_y < 1) break;
                      for (i=0;i<_NumberOfItems;i++)
                      {
                         char Hotkey;
                         Hotkey = ExtractHotkey ( _pItem[i].Description, FALSE );
                         if (ch == toupper(Hotkey))
                         {
                            _SelectedItemNumber = i;
                            return(0);
                         }
                      }
      }
   }
}

void tMenu::menu_background ( void )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                          ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   _put_attr(1,1,2000,_BackgroundColour);
   _put_char(1,1,2000,_BackgroundCharacter[0]);
}

void tMenu::menu_header ( void )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                          ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int x,y,delta_x,delta_y;

   delta_x = strlen(_Heading);
   delta_y = 1;
   delta_x += 4;
   delta_y += 2;
   x = (getvcols() - delta_x)/2;
   y = 2;

   clrwin(x,y,x+delta_x-1,y+delta_y-1,_NormalColour);
   box(x,y,x+delta_x-1,y+delta_y-1,_HighlightColour,3);
   shadow(x,y,x+delta_x-1,y+delta_y-1,_ShadowColour,1);
   _put_string(x+2,y+1,_Heading);
}

int tMenu::menu_width ( void )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int delta_x = 0;
   int i;
   int len;
   char Description[81];

   for (i=0;i<_NumberOfItems;i++)
   {
      strcpy ( Description, _pItem[i].Description );
      ExtractHotkey ( Description, TRUE );
      len = (int)strlen(Description);
      if (len > delta_x) delta_x = len;
   }
   return(delta_x);
}

void tMenu::MenuSetItem ( int ItemNumber, char *Description )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                         ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   if (_pItem == NULL) return;
   memset( &_pItem[ItemNumber], 0, sizeof(MENU_OPTION) );
   if (Description)
      strcpy(_pItem[ItemNumber].Description, Description);
}

//char *tMenu::EmbedHotkey ( char *Hotkey, char *Description )
////ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
////³
////ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//{
//   char *p;
//   static char Result[128];
//   strcpy(Result,Description);
//   p = strchr ( Result, Hotkey[0] );
//   if (p==NULL)
//   {
//      p = strchr ( Result, toupper(Hotkey[0]) );
//      if (p==NULL)
//      {
//         p = strchr ( Result, tolower(Hotkey[0]) );
//         if (p==NULL)
//            return Result;
//      }
//   }
//
//   memmove(p+1,p,strlen(p)+1);
//   *p = '&';
//   return Result;
//}

#ifdef DEBUG
void tMenu::Debug ( void )
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³                                                          ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
{
   int i,j,k;
   char Str[81];

   k = 1;

   if (_pItem == NULL) return;
   sprintf(Str, "==>%s<==",_Heading);
   _put_text(1,k++,0x70,Str);

   for (j=0; j<_NumberOfItems; j++)
   {
      sprintf(Str, "   %2d : %2d ==>%s<==",j,_pItem[j].SubMenuNumber,_pItem[j].Description);
      _put_text(1,k++,0x70,Str);
   }
   getch();
}
#endif
