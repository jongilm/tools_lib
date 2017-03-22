
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>

#include <tools.h>

static void process_special_char(char *line);
static void process_normal_char(char *line, char *pic);
static void test_insert(void);
static int  check_picture(char ch, char picch);
static void replace_char(char *line, int k, char ch);
static void insert_char(char *line, int k, char ch);
static void delete_char(char *line, int k);
static void process_control_char(char *line);
static void process_extended_char(char *line);

static int ch;
static int i;           /* current position    */
static int nulpos;      /* position of nul at end of string */
static int proceed;
static int extended;
static int insert = FALSE;

int edline(char *line, char *pic, unsigned int colour)
/**************************************/
/*                                    */
/**************************************/
{
   int x;
   int y;
   int z;
   char temp[2];

   x = wherex();
   y = wherey();
   test_insert();

   nulpos = strlen(pic);     /* position of nul */
   line[nulpos] = 0;
   for (i=strlen(line);i<nulpos;i++) line[i] = ' ';
   i = 0;

   proceed = TRUE;
   while (proceed)
   {
      temp[1] = '\0';
      for (z=0;z<nulpos;z++)
      {
         if ((pic[z] == '#') && (line[z] != ' ')) temp[0] = '#';
         else                                     temp[0] = line[z];
         _put_text(x+z,y,colour,temp);
      }

      gotoxy(x+i,y);

      ch = getch();

      if ((ch==127) || ((ch>=0) && (ch<= 31)))
         process_special_char(line);
      else
         process_normal_char(line,pic);
   /*   if (isprint(ch)) process_normal_char(line,pic);
      else             process_special_char(line);     */
   }
   for (i=strlen(line)-1;line[i]==' ';i--) line[i] = '\0';
   setcursorshape(1);
   if (extended) ch = -ch;
   return(ch);
}

static void test_insert(void)
/**************************************/
/*                                    */
/**************************************/
{
   if (insert) setcursorshape(3);
   else        setcursorshape(1);
   return;
}

static void process_normal_char(char *line, char *pic)
/**************************************/
/*                                    */
/**************************************/
{
   if (i < nulpos)
   {
      ch = check_picture((char)ch,pic[i]);
      if (ch)
         if (insert) insert_char(line,i,(char)ch);
         else        replace_char(line,i,(char)ch);
   }
   return;
}

static void process_special_char(char *line)
/**************************************/
/*                                    */
/**************************************/
{
   if (!ch)
      process_extended_char(line);
   else
      process_control_char(line);
   return;
}

static void process_extended_char(char *line)
/**************************************/
/*                                    */
/**************************************/
{
   extended = 1;
   ch = getch();
   switch(ch)
   {
      case 'K': if (i) i--;                                /* LFT */
                break;
      case 'M': if (i < nulpos-1) i++;                     /* RGT */
                break;
      case 'R': insert = (!insert);                        /* INS */
                test_insert();
                break;
      case 'S': delete_char(line,i);                       /* DEL */
                break;
      case 'G': i = 0;                                     /* HOM */
                break;
      case 'O': i = nulpos-1;                              /* END */
                break;
      default : proceed = FALSE;                           /* all others */
   }
   return;
}

static void process_control_char(char *line)
/**************************************/
/*                                    */
/**************************************/
{
   extended = 0;
   switch(ch)
   {  case 8  : if (i)                                     /* BS  */
                {
                   if (insert) delete_char(line,i-1);
                   i--;
                }
                break;
      default : proceed = FALSE;                           /* all others */
   }
   return;
}

static void insert_char(char *line, int k, char ch)
/**************************************/
/*                                    */
/**************************************/
{
   int j;
   if (k < nulpos-1)
      for (j = nulpos-1; j > k; j--) line[j] = line[j-1];
   line[k] = ch;

   if (i < nulpos-1) i++;
   return;
}

static void replace_char(char *line, int k, char ch)
/**************************************/
/*                                    */
/**************************************/
{
   if ((k >= 0) || (k <= nulpos-1))
      line[k] = ch;

   if (i < nulpos-1) i++;
   return;
}

static void delete_char(char *line, int k)
/**************************************/
/*                                    */
/**************************************/
{
   int j;
   if (i < nulpos)
      for (j = k; j < nulpos-1; j++) line[j] = line[j+1];
   line[nulpos-1] = ' ';
   return;
}

static int check_picture(char ch, char picch)
/**************************************/
/*                                    */
/**************************************/
{
   int retcode;
   retcode = ch;
   switch (picch)
   {
      case 'A': break;
      case 'X': {
                   if ((ch < ' ') || (ch > '~'))
                      retcode = 0;
                   break;
                }
      case '#':
      case '!': {
                   retcode = toupper(ch);
                   if ((ch < ' ') || (ch > '~'))
                      retcode = 0;
                   break;
                }
      case '9': {
                   if (((ch < '0') || (ch > '9'))
                   && (ch != '.')
                   && (ch != ' ')
                   && (ch != '+')
                   && (ch != '-'))
                      retcode = 0;
                   break;
                }
   }
   return(retcode);
}

