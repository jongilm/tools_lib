
#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE          1
#define FALSE         0
#endif

#ifndef YES
#define YES           1
#define NO            0
#endif

#ifndef MK_FP
#define MK_FP(seg,ofs)  ((void far *)(((unsigned long)(seg) << 16) | (ofs)))
#endif

#ifndef PEEKW
#define PEEKW(a,b)      (*((int far*)MK_FP((a),(b))))
#endif

#ifndef HLB
#define HLB(h,l) h*256+l  /* assign byte values to integer 16 bit register */
#define HB(x) x/256       /* extract high byte */
#define LB(x) x%256       /* extract low byte */
#endif

#ifndef MAXPAGE
#define MAXPAGE     8            /* Max video pages            */
#endif
#ifndef SCROLL_DOWN
#define SCROLL_DOWN 0x07         /* Scroll direction           */
#endif
#ifndef SCROLL_UP
#define SCROLL_UP   0x06
#endif

#ifndef BLACK
#define BLACK     0x00
#endif
#ifndef BLUE
#define BLUE      0x01
#endif
#ifndef GREEN
#define GREEN     0x02
#endif
#ifndef CYAN
#define CYAN      0x03
#endif
#ifndef RED
#define RED       0x04
#endif
#ifndef MAGENTA
#define MAGNETA   0x05
#endif
#ifndef BROWN
#define BROWN     0x06
#endif
#ifndef WHITE
#define WHITE     0x07
#endif
#ifndef LIGHT
#define LIGHT     0x08
#endif
#ifndef YELLOW
#define YELLOW    0x0E   /* = LIGHT + BROWN                    */
#endif

#define ENTER         0x0D
#define ESCAPE        0x1B

typedef struct
{
   char _far *scrnbufptr;
   int x1, y1, x2, y2;
   int window_colour;
   int box_colour;
   int box_type;
   int shadow_colour;
   int shadow_type;
} window_t;

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ SCR_IO.ASM                                              ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
extern int _far _cdecl _scr_seg;
extern int _far _cdecl _scr_cols;
extern int _far _cdecl _scr_snow;

void       _far _cdecl _scr_init   (void);
void       _far _cdecl _put_char   ( int x, int y, int qty, int ch);
void       _far _cdecl _put_attr   ( int x, int y, int qty, int colour);
int        _far _cdecl _get_char   ( int x, int y);
int        _far _cdecl _get_attr   ( int x, int y);
void       _far _cdecl _put_chars  ( int x, int y, int length, char _far *str);
void       _far _cdecl _put_attrs  ( int x, int y, int length, char _far *str);
void       _far _cdecl _get_chars  ( int x, int y, int length, char _far *str);
void       _far _cdecl _get_attrs  ( int x, int y, int length, char _far *str);
void       _far _cdecl _put_string ( int x, int y, char _far *zstr);
void       _far _cdecl _put_video  ( int x, int y, int length, char _far *str);
void       _far _cdecl _get_video  ( int x, int y, int length, char _far *str);
void       _far _cdecl _put_field  ( int x, int y, int colour, int fillcolour, int length, char _far *str);
void       _far _cdecl _put_text   ( int x, int y, int colour, char _far *zstr);
void       _far _cdecl _put_chars_xp( int x, int y, int length, char _far *str);
void       _far _cdecl _put_attrs_xp( int x, int y, int length, char _far *str);

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ SCR.C                                                   ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
void              _cdecl clrscr(int color);
void              _cdecl clrbos(int x, int y, int colour);
void              _cdecl clreos(int x, int y, int colour);
void              _cdecl clrl(int y, int colour);
void              _cdecl clrbol(int x, int y, int colour);
void              _cdecl clreol(int x, int y, int colour);
void              _cdecl clrwin(int x1, int y1, int x2, int y2, int colour);
void              _cdecl scroll(int x, int y, int dx, int dy, int num, int function, int colour);
void              _cdecl setborder(int colour);
void              _cdecl getcursorsize(int *start, int *end);
void              _cdecl setcursorsize(int start, int end);
void              _cdecl getcursorshape(int *start, int *end);
void              _cdecl setcursorshape(int type);
int               _cdecl getvcols(void);
int               _cdecl getvdev(void);
int               _cdecl getvmode(void);
void              _cdecl setvmode(int mode);
void              _cdecl setvgablink(int blink);
int               _cdecl getvpage(void);
void              _cdecl setvpage(int page);
unsigned int      _cdecl getvseg(void);
int               _cdecl getvsnow(void);
int               _cdecl getvgamono(void);
int               _cdecl wherex(void);
int               _cdecl wherey(void);
void              _cdecl gotoxy(int x, int y);

void              _cdecl box(int x1,int y1,int x2,int y2,int colour,int type);
char _far       * _cdecl pushscreen(void);
void              _cdecl popscreen(char _far *scrnbufptr);
char _far       * _cdecl pushwindow(int x1,int y1,int x2,int y2);
void              _cdecl popwindow(char _far *scrnbufptr,int x1,int y1,int x2,int y2);
void              _cdecl shadow(int x1,int y1,int x2,int y2,int colour,int type);
void              _cdecl beep(unsigned int freq,unsigned int length);
void              _cdecl pip(void);
void              _cdecl window_up(window_t *w_ptr);
void              _cdecl window_dn(window_t *w_ptr);

int               _cdecl windowstorage(int x1,int y1,int x2,int y2);
void              _cdecl pushwindow_noalloc(char _far *scrnbufptr,int x1,int y1,int x2,int y2);
void              _cdecl popwindow_noalloc(char _far *scrnbufptr,int x1,int y1,int x2,int y2);
void              _cdecl window_up_noalloc(window_t *w_ptr);
void              _cdecl window_dn_noalloc(window_t *w_ptr);

int               _cdecl edline(char *line, char *pic, unsigned int colour);
int               _cdecl ReadField( int x, int y, int colour, int (*validationfunction)(char *field), char *string, char *picture);

int               _cdecl PressAnyKey ( int Colour, int ShadowColour );
unsigned long     _cdecl GetDiskFreeSpace ( int drive );


/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ DELAY.C                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
extern unsigned long delay_reference;
extern void delay (int Ticks);
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ BEEP.C                                                  ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
void beep  ( unsigned int freq, unsigned int length );
void pip   ( void );
/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ TVM.C                                                   ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/

/*
   double PV;           Present Value
   double FV;           Future Value
   double I;            Interest rate per year as a percentage
   double N;            Period in months
   double PMT;          Payment
   double P;            payment at beginning(1) or end(0) of each period
*/

#define SAVINGS  1
#define MORTGAGE 0
#define round(x) (((x+0.005)*100)/100)

double tvm_pmt(double PV,double FV,double I,double N,double PMT,double P);
double tvm_pv (double PV,double FV,double I,double N,double PMT,double P);
double tvm_fv (double PV,double FV,double I,double N,double PMT,double P);
double tvm_n  (double PV,double FV,double I,double N,double PMT,double P);
double tvm_i  (double PV,double FV,double I,double N,double PMT,double P);


/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ DIR_SCAN.C                                              ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
void dir_scan(void (*diskfunction)(void),void (*dirfunction)(void));

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ COLOUR.C                                                ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
#define NO_COLOUR_SELECTED -1
int get_new_colour(char *heading, int oldcolour);


/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ FIELD.C                                                 ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
#pragma pack(1)

typedef struct
{
   int  x;
   int  y;
   char prompt[81];
   int  type;
   char picture[81];
   void *pdata;
} fieldtype;

#pragma pack()

void SetFieldColours ( int Prompt, int Selected, int Unselected );
void SetFieldInfo( int ndx, int x, int y, char *prompt, int type, char *picture, void *pdata);
void DisplayFields(void);
int EditField(int ndx);
char *Pic(int ch, int length);
int EditFieldAlpha(int x, int y, char *prompt, char *picture, char *zstr);
int EditFieldNumeric(int x, int y, char *prompt, char *picture, int *value);
int EditFieldHex(int x, int y, char *prompt, char *picture, char *value);


/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ DES.H                                                           ³*/
/*³                                                                 ³*/
/*³ DES: Duplicate the NBS Data Encryption Standard in software.    ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/

/* The values at key, inblock, and infile are preserved.*/

/* endesf and dedesf have the following return codes :  */
/*            0 : successful                            */
/*            1 : error opening infile for reading      */
/*            2 : error opening outfile for writing     */
/*            3 : error reading infile                  */
/*            4 : error writing outfile                 */

void desinit(char *key);                    /* initialize with 64-bit key   */
void endes(char *inblock,char *outblock);   /* encrypt 64-bit inblock       */
void dedes(char *inblock,char *outblock);   /* decrypt 64-bit inblock       */
int endesf(char *infile,char *outfile);     /* encrypt infile               */
int dedesf(char *infile,char *outfile);     /* decrypt infile               */

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ BTRV.C                                                  ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/

int   load_btrv(void);
int   unload_btrv(void);
char *b_err(int err_num, char *err_msg);

int   btrv(int OP,char *POS_BLK,char *DATA_BUF,int *DATA_LEN,char *KEY_BUF,int KEY_NUM);

int open_file(char *posblk, char *filename);
int close_file(char *posblk);

typedef struct
{
   int  key_pos;
   int  key_len;
   int  key_flag;
   char not_used[4];
   char key_type;
   char reserved[5];
} B_KEYSPEC;

typedef struct
{
   int       rec_len;
   int       page_size;
   int       ndx_count;
   char      not_used[4];
   int       file_flag;
   char      reserved[2];
   int       pre_alloc;
} B_FILESPEC;

/* Use as follows */
/*
   struct
   {   B_FILESPEC  body;
       B_KEYSPEC   key[3];
   } file_spec;
*/

#define B_OPEN      0
#define B_CLOSE     1
#define B_INSERT    2
#define B_UPDATE    3
#define B_DELETE    4
#define B_GETEQ     5
#define B_GETNEXT   6
#define B_GETPREV   7
#define B_GETGR     8
#define B_GETGREQ   9
#define B_GETLESS   10
#define B_GETLESSEQ 11
#define B_GETFIRST  12
#define B_GETLAST   13
#define B_CREATE    14
#define B_STAT      15
#define B_EXTEND    16
#define B_SETDIR    17
#define B_GETDIR    18
#define B_BTRANS    19
#define B_ETRANS    20
#define B_ATRANS    21
#define B_GETPOS    22
#define B_GDIRECT   23
#define B_SDIRECT   24
#define B_STOP      25
#define B_VERS      26
#define B_UNLOCK    27
#define B_RESET     28
#define B_SETOWN    29
#define B_CLROWN    30
#define B_CRSUPP    31
#define B_DRSUPP    32


/* btrieve extended key types */

#define B_STRING    0
#define B_INT       1
#define B_FLOAT     2
#define B_DATE      3
#define B_TIME      4
#define B_DECIMAL   5
#define B_MONEY     6
#define B_LOGICAL   7
#define B_NUMERIC   8
#define B_BFLOAT    9
#define B_LSTRING   10
#define B_ZSTRING   11
#define B_UNSIGN    14

/* btrieve file attributes */

#define B_VARIABLE  1
#define B_BLNKCOMP  2
#define B_PREALLOC  4

/* btrieve key attributes */

#define B_DUPL      1
#define B_MOD       2
#define B_BIN       4
#define B_NULL      8
#define B_SEG       16
#define B_ALT       32
#define B_DESC      64
#define B_SUPPL     128
#define B_EXT       256


/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ SCANCODES                                               ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
#ifdef SCANCODES
/*
  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

Keyboard Codes

    This tables lists all the keyboard codes that are returned by calls to
    INT 16 functions 0 through 2 (the second and third columns) and 10h
    through 12h (the last column).

      Key 45  In the United States, the 101/102-key keyboard is shipped
              with 101 keys. Overseas versions have an additional key
              sandwiched between the left Shift key and the Z key. This
              additional key is identified by IBM (and in this table) as
              "Key 45."

       Flag   1 = No key.
              2 = Ignored by ROM BIOS.
              4 = Used internally by ROM BIOS. No values in keyboard buffer.
              8 = Used to generate decimal ascii codes on the numeric keypad.

  ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
*/

typedef struct
{
   char Description[26];

   unsigned int Flag_083_00;      /* The 83/84-key "Standard" keyboard        */
   unsigned int AX_083_Std;       /* Interrupt 16h Services 0, 1, and 2.      */

   unsigned int Flag_101_00;      /* The 101/102-key "Enhanced" keyboard.     */
   unsigned int AX_101_Std;       /* Interrupt 16h Services 0, 1, and 2.      */

   unsigned int Flag_101_10;      /* The 101/102-key "Enhanced" keyboard.     */
   unsigned int AX_101_Enh;       /* Interrupt 16h Services 10h, 11h, and 12h.*/
} ScanCodeType;

#define SCANCODES 396
ScanCodeType ScanCode[SCANCODES+1] =
/*   Description                 83/84-Key      101/102-Key         */
/*                               Standard   Standard   Extended     */
{
    "Esc"                     ,  0,0x011B,  0,0x011B,  0,0x011B,
    "1"                       ,  0,0x0231,  0,0x0231,  0,0x0231,
    "2"                       ,  0,0x0332,  0,0x0332,  0,0x0332,
    "3"                       ,  0,0x0433,  0,0x0433,  0,0x0433,
    "4"                       ,  0,0x0534,  0,0x0534,  0,0x0534,
    "5"                       ,  0,0x0635,  0,0x0635,  0,0x0635,
    "6"                       ,  0,0x0736,  0,0x0736,  0,0x0736,
    "7"                       ,  0,0x0837,  0,0x0837,  0,0x0837,
    "8"                       ,  0,0x0938,  0,0x0938,  0,0x0938,
    "9"                       ,  0,0x0A39,  0,0x0A39,  0,0x0A39,
    "0"                       ,  0,0x0B30,  0,0x0B30,  0,0x0B30,
    "-"                       ,  0,0x0C2D,  0,0x0C2D,  0,0x0C2D,
    "="                       ,  0,0x0D3D,  0,0x0D3D,  0,0x0D3D,
    "Backspace"               ,  0,0x0E08,  0,0x0E08,  0,0x0E08,
    "Tab"                     ,  0,0x0F09,  0,0x0F09,  0,0x0F09,
    "q"                       ,  0,0x1071,  0,0x1071,  0,0x1071,
    "w"                       ,  0,0x1177,  0,0x1177,  0,0x1177,
    "e"                       ,  0,0x1265,  0,0x1265,  0,0x1265,
    "r"                       ,  0,0x1372,  0,0x1372,  0,0x1372,
    "t"                       ,  0,0x1474,  0,0x1474,  0,0x1474,
    "y"                       ,  0,0x1579,  0,0x1579,  0,0x1579,
    "u"                       ,  0,0x1675,  0,0x1675,  0,0x1675,
    "i"                       ,  0,0x1769,  0,0x1769,  0,0x1769,
    "o"                       ,  0,0x186F,  0,0x186F,  0,0x186F,
    "p"                       ,  0,0x1970,  0,0x1970,  0,0x1970,
    "["                       ,  0,0x1A5B,  0,0x1A5B,  0,0x1A5B,
    "]"                       ,  0,0x1B5D,  0,0x1B5D,  0,0x1B5D,
    "Enter"                   ,  0,0x1C0D,  0,0x1C0D,  0,0x1C0D,
    "Ctrl"                    ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "a"                       ,  0,0x1E61,  0,0x1E61,  0,0x1E61,
    "s"                       ,  0,0x1F73,  0,0x1F73,  0,0x1F73,
    "d"                       ,  0,0x2064,  0,0x2064,  0,0x2064,
    "f"                       ,  0,0x2166,  0,0x2166,  0,0x2166,
    "g"                       ,  0,0x2267,  0,0x2267,  0,0x2267,
    "h"                       ,  0,0x2368,  0,0x2368,  0,0x2368,
    "j"                       ,  0,0x246A,  0,0x246A,  0,0x246A,
    "k"                       ,  0,0x256B,  0,0x256B,  0,0x256B,
    "l"                       ,  0,0x266C,  0,0x266C,  0,0x266C,
    ";"                       ,  0,0x273B,  0,0x273B,  0,0x273B,
    "'"                       ,  0,0x2827,  0,0x2827,  0,0x2827,
    "`"                       ,  0,0x2960,  0,0x2960,  0,0x2960,
    "Shift"                   ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "\\"                      ,  0,0x2B5C,  0,0x2B5C,  0,0x2B5C,
    "z"                       ,  0,0x2C7A,  0,0x2C7A,  0,0x2C7A,
    "x"                       ,  0,0x2D78,  0,0x2D78,  0,0x2D78,
    "c"                       ,  0,0x2E63,  0,0x2E63,  0,0x2E63,
    "v"                       ,  0,0x2F76,  0,0x2F76,  0,0x2F76,
    "b"                       ,  0,0x3062,  0,0x3062,  0,0x3062,
    "n"                       ,  0,0x316E,  0,0x316E,  0,0x316E,
    "m"                       ,  0,0x326D,  0,0x326D,  0,0x326D,
    ","                       ,  0,0x332C,  0,0x332C,  0,0x332C,
    "."                       ,  0,0x342E,  0,0x342E,  0,0x342E,
    "/"                       ,  0,0x352F,  0,0x352F,  0,0x352F,
    "Gray *"                  ,  0,0x372A,  0,0x372A,  0,0x372A,
    "Alt"                     ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Space"                   ,  0,0x3920,  0,0x3920,  0,0x3920,
    "Caps Lock"               ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "F1"                      ,  0,0x3B00,  0,0x3B00,  0,0x3B00,
    "F2"                      ,  0,0x3C00,  0,0x3C00,  0,0x3C00,
    "F3"                      ,  0,0x3D00,  0,0x3D00,  0,0x3D00,
    "F4"                      ,  0,0x3E00,  0,0x3E00,  0,0x3E00,
    "F5"                      ,  0,0x3F00,  0,0x3F00,  0,0x3F00,
    "F6"                      ,  0,0x4000,  0,0x4000,  0,0x4000,
    "F7"                      ,  0,0x4100,  0,0x4100,  0,0x4100,
    "F8"                      ,  0,0x4200,  0,0x4200,  0,0x4200,
    "F9"                      ,  0,0x4300,  0,0x4300,  0,0x4300,
    "F10"                     ,  0,0x4400,  0,0x4400,  0,0x4400,
    "F11"                     ,  1,0xFFFF,  2,0xFFFF,  0,0x8500,
    "F12"                     ,  1,0xFFFF,  2,0xFFFF,  0,0x8600,
    "Num Lock"                ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Scroll Lock"             ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "White Home"              ,  0,0x4700,  0,0x4700,  0,0x4700,
    "White Up Arrow"          ,  0,0x4800,  0,0x4800,  0,0x4800,
    "White PgUp"              ,  0,0x4900,  0,0x4900,  0,0x4900,
    "Gray -"                  ,  0,0x4A2D,  0,0x4A2D,  0,0x4A2D,
    "White Left Arrow"        ,  0,0x4B00,  0,0x4B00,  0,0x4B00,
    "Center Key"              ,  2,0xFFFF,  2,0xFFFF,  0,0x4C00,
    "White Right Arrow"       ,  0,0x4D00,  0,0x4D00,  0,0x4D00,
    "Gray +"                  ,  0,0x4E2B,  0,0x4E2B,  0,0x4E2B,
    "White End"               ,  0,0x4F00,  0,0x4F00,  0,0x4F00,
    "White Down Arrow"        ,  0,0x5000,  0,0x5000,  0,0x5000,
    "White PgDn"              ,  0,0x5100,  0,0x5100,  0,0x5100,
    "White Ins"               ,  0,0x5200,  0,0x5200,  0,0x5200,
    "White Del"               ,  0,0x5300,  0,0x5300,  0,0x5300,
    "SysReq"                  ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Key 45"                  ,  1,0xFFFF,  0,0x565C,  0,0x565C,
    "Enter (number keypad)"   ,  1,0xFFFF,  0,0x1C0D,  0,0xE00D,
    "Gray /"                  ,  1,0xFFFF,  0,0x352F,  0,0xE02F,
    "PrtSc"                   ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Pause"                   ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Gray Home"               ,  1,0xFFFF,  0,0x4700,  0,0x47E0,
    "Gray Up Arrow"           ,  1,0xFFFF,  0,0x4800,  0,0x48E0,
    "Gray Page Up"            ,  1,0xFFFF,  0,0x4900,  0,0x49E0,
    "Gray Left Arrow"         ,  1,0xFFFF,  0,0x4B00,  0,0x4BE0,
    "Gray Right Arrow"        ,  1,0xFFFF,  0,0x4D00,  0,0x4DE0,
    "Gray End"                ,  1,0xFFFF,  0,0x4F00,  0,0x4FE0,
    "Gray Down Arrow"         ,  1,0xFFFF,  0,0x5000,  0,0x50E0,
    "Gray Page Down"          ,  1,0xFFFF,  0,0x5100,  0,0x51E0,
    "Gray Insert"             ,  1,0xFFFF,  0,0x5200,  0,0x52E0,
    "Gray Delete"             ,  1,0xFFFF,  0,0x5300,  0,0x53E0,
    "Shift Esc"               ,  0,0x011B,  0,0x011B,  0,0x011B,
    "!"                       ,  0,0x0221,  0,0x0221,  0,0x0221,
    "@"                       ,  0,0x0340,  0,0x0340,  0,0x0340,
    "#"                       ,  0,0x0423,  0,0x0423,  0,0x0423,
    "$"                       ,  0,0x0524,  0,0x0524,  0,0x0524,
    "%%"                      ,  0,0x0625,  0,0x0625,  0,0x0625,
    "^"                       ,  0,0x075E,  0,0x075E,  0,0x075E,
    "&"                       ,  0,0x0826,  0,0x0826,  0,0x0826,
    "* (white)"               ,  0,0x092A,  0,0x092A,  0,0x092A,
    "("                       ,  0,0x0A28,  0,0x0A28,  0,0x0A28,
    ")"                       ,  0,0x0B29,  0,0x0B29,  0,0x0B29,
    "_"                       ,  0,0x0C5F,  0,0x0C5F,  0,0x0C5F,
    "+ (white)"               ,  0,0x0D2B,  0,0x0D2B,  0,0x0D2B,
    "Shift Backspace"         ,  0,0x0E08,  0,0x0E08,  0,0x0E08,
    "Shift Tab (Backtab)"     ,  0,0x0F00,  0,0x0F00,  0,0x0F00,
    "Q"                       ,  0,0x1051,  0,0x1051,  0,0x1051,
    "W"                       ,  0,0x1157,  0,0x1157,  0,0x1157,
    "E"                       ,  0,0x1245,  0,0x1245,  0,0x1245,
    "R"                       ,  0,0x1352,  0,0x1352,  0,0x1352,
    "T"                       ,  0,0x1454,  0,0x1454,  0,0x1454,
    "Y"                       ,  0,0x1559,  0,0x1559,  0,0x1559,
    "U"                       ,  0,0x1655,  0,0x1655,  0,0x1655,
    "I"                       ,  0,0x1749,  0,0x1749,  0,0x1749,
    "O"                       ,  0,0x184F,  0,0x184F,  0,0x184F,
    "P"                       ,  0,0x1950,  0,0x1950,  0,0x1950,
    "{"                       ,  0,0x1A7B,  0,0x1A7B,  0,0x1A7B,
    "}"                       ,  0,0x1B7D,  0,0x1B7D,  0,0x1B7D,
    "Shift Enter"             ,  0,0x1C0D,  0,0x1C0D,  0,0x1C0D,
    "Shift Ctrl"              ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "A"                       ,  0,0x1E41,  0,0x1E41,  0,0x1E41,
    "S"                       ,  0,0x1F53,  0,0x1F53,  0,0x1F53,
    "D"                       ,  0,0x2044,  0,0x2044,  0,0x2044,
    "F"                       ,  0,0x2146,  0,0x2146,  0,0x2146,
    "G"                       ,  0,0x2247,  0,0x2247,  0,0x2247,
    "H"                       ,  0,0x2348,  0,0x2348,  0,0x2348,
    "J"                       ,  0,0x244A,  0,0x244A,  0,0x244A,
    "K"                       ,  0,0x254B,  0,0x254B,  0,0x254B,
    "L"                       ,  0,0x264C,  0,0x264C,  0,0x264C,
    ":"                       ,  0,0x273A,  0,0x273A,  0,0x273A,
    "\""                      ,  0,0x2822,  0,0x2822,  0,0x2822,
    "~"                       ,  0,0x297E,  0,0x297E,  0,0x297E,
    "|"                       ,  0,0x2B7C,  0,0x2B7C,  0,0x2B7C,
    "Z"                       ,  0,0x2C5A,  0,0x2C5A,  0,0x2C5A,
    "X"                       ,  0,0x2D58,  0,0x2D58,  0,0x2D58,
    "C"                       ,  0,0x2E43,  0,0x2E43,  0,0x2E43,
    "V"                       ,  0,0x2F56,  0,0x2F56,  0,0x2F56,
    "B"                       ,  0,0x3042,  0,0x3042,  0,0x3042,
    "N"                       ,  0,0x314E,  0,0x314E,  0,0x314E,
    "M"                       ,  0,0x324D,  0,0x324D,  0,0x324D,
    "<"                       ,  0,0x333C,  0,0x333C,  0,0x333C,
    ">"                       ,  0,0x343E,  0,0x343E,  0,0x343E,
    "?"                       ,  0,0x353F,  0,0x353F,  0,0x353F,
    "Shift Gray *"            ,  4,0xFFFF,  4,0xFFFF,  0,0x372A,
    "Shift Alt"               ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Shift Space"             ,  0,0x3920,  0,0x3920,  0,0x3920,
    "Shift Caps Lock"         ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Shift F1"                ,  0,0x5400,  0,0x5400,  0,0x5400,
    "Shift F2"                ,  0,0x5500,  0,0x5500,  0,0x5500,
    "Shift F3"                ,  0,0x5600,  0,0x5600,  0,0x5600,
    "Shift F4"                ,  0,0x5700,  0,0x5700,  0,0x5700,
    "Shift F5"                ,  0,0x5800,  0,0x5800,  0,0x5800,
    "Shift F6"                ,  0,0x5900,  0,0x5900,  0,0x5900,
    "Shift F7"                ,  0,0x5A00,  0,0x5A00,  0,0x5A00,
    "Shift F8"                ,  0,0x5B00,  0,0x5B00,  0,0x5B00,
    "Shift F9"                ,  0,0x5C00,  0,0x5C00,  0,0x5C00,
    "Shift F10"               ,  0,0x5D00,  0,0x5D00,  0,0x5D00,
    "Shift F11"               ,  1,0xFFFF,  2,0xFFFF,  0,0x8700,
    "Shift F12"               ,  1,0xFFFF,  2,0xFFFF,  0,0x8800,
    "Shift Num Lock"          ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Shift Scroll Lock"       ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Shift 7 (number pad)"    ,  0,0x4737,  0,0x4737,  0,0x4737,
    "Shift 8 (number pad)"    ,  0,0x4838,  0,0x4838,  0,0x4838,
    "Shift 9 (number pad)"    ,  0,0x4939,  0,0x4939,  0,0x4939,
    "Shift Gray -"            ,  0,0x4A2D,  0,0x4A2D,  0,0x4A2D,
    "Shift 4 (number pad)"    ,  0,0x4B34,  0,0x4B34,  0,0x4B34,
    "Shift 5 (number pad)"    ,  0,0x4C35,  0,0x4C35,  0,0x4C35,
    "Shift 6 (number pad)"    ,  0,0x4D36,  0,0x4D36,  0,0x4D36,
    "Shift Gray +"            ,  0,0x4E2B,  0,0x4E2B,  0,0x4E2B,
    "Shift 1 (number pad)"    ,  0,0x4F31,  0,0x4F31,  0,0x4F31,
    "Shift 2 (number pad)"    ,  0,0x5032,  0,0x5032,  0,0x5032,
    "Shift 3 (number pad)"    ,  0,0x5133,  0,0x5133,  0,0x5133,
    "Shift 0 (number pad)"    ,  0,0x5230,  0,0x5230,  0,0x5230,
    "Shift . (number pad)"    ,  0,0x532E,  0,0x532E,  0,0x532E,
    "Shift SysReq"            ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Shift Key 45"            ,  1,0xFFFF,  0,0x567C,  0,0x567C,
    "Shift Enter (number pad)",  1,0xFFFF,  0,0x1C0D,  0,0xE00D,
    "Shift Gray /"            ,  1,0xFFFF,  0,0x352F,  0,0xE02F,
    "Shift PrtSc"             ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Shift Pause"             ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Shift Gray Home"         ,  1,0xFFFF,  0,0x4700,  0,0x47E0,
    "Shift Gray Up Arrow"     ,  1,0xFFFF,  0,0x4800,  0,0x48E0,
    "Shift Gray Page Up"      ,  1,0xFFFF,  0,0x4900,  0,0x49E0,
    "Shift Gray Left Arrow"   ,  1,0xFFFF,  0,0x4B00,  0,0x4BE0,
    "Shift Gray Right Arrow"  ,  1,0xFFFF,  0,0x4D00,  0,0x4DE0,
    "Shift Gray End"          ,  1,0xFFFF,  0,0x4F00,  0,0x4FE0,
    "Shift Gray Down Arrow"   ,  1,0xFFFF,  0,0x5000,  0,0x50E0,
    "Shift Gray Page Down"    ,  1,0xFFFF,  0,0x5100,  0,0x51E0,
    "Shift Gray Insert"       ,  1,0xFFFF,  0,0x5200,  0,0x52E0,
    "Shift Gray Delete"       ,  1,0xFFFF,  0,0x5300,  0,0x53E0,
    "Ctrl Esc"                ,  0,0x011B,  0,0x011B,  0,0x011B,
    "Ctrl 1"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl 2 (NUL)"            ,  0,0x0300,  0,0x0300,  0,0x0300,
    "Ctrl 3"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl 4"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl 5"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl 6 (RS)"             ,  0,0x071E,  0,0x071E,  0,0x071E,
    "Ctrl 7"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl 8"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl 9"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl 0"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl -"                  ,  0,0x0C1F,  0,0x0C1F,  0,0x0C1F,
    "Ctrl ="                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl Backspace (DEL)"    ,  0,0x0E7F,  0,0x0E7F,  0,0x0E7F,
    "Ctrl Tab"                ,  2,0xFFFF,  2,0xFFFF,  0,0x9400,
    "Ctrl q (DC1)"            ,  0,0x1011,  0,0x1011,  0,0x1011,
    "Ctrl w (ETB)"            ,  0,0x1117,  0,0x1117,  0,0x1117,
    "Ctrl e (ENQ)"            ,  0,0x1205,  0,0x1205,  0,0x1205,
    "Ctrl r (DC2)"            ,  0,0x1312,  0,0x1312,  0,0x1312,
    "Ctrl t (DC4)"            ,  0,0x1414,  0,0x1414,  0,0x1414,
    "Ctrl y (EM)"             ,  0,0x1519,  0,0x1519,  0,0x1519,
    "Ctrl u (NAK)"            ,  0,0x1615,  0,0x1615,  0,0x1615,
    "Ctrl i (HT)"             ,  0,0x1709,  0,0x1709,  0,0x1709,
    "Ctrl o (SI)"             ,  0,0x180F,  0,0x180F,  0,0x180F,
    "Ctrl p (DEL)"            ,  0,0x1910,  0,0x1910,  0,0x1910,
    "Ctrl [ (ESC)"            ,  0,0x1A1B,  0,0x1A1B,  0,0x1A1B,
    "Ctrl ] (GS)"             ,  0,0x1B1D,  0,0x1B1D,  0,0x1B1D,
    "Ctrl Enter (LF)"         ,  0,0x1C0A,  0,0x1C0A,  0,0x1C0A,
    "Ctrl a (SOH)"            ,  0,0x1E01,  0,0x1E01,  0,0x1E01,
    "Ctrl s (DC3)"            ,  0,0x1F13,  0,0x1F13,  0,0x1F13,
    "Ctrl d (EOT)"            ,  0,0x2004,  0,0x2004,  0,0x2004,
    "Ctrl f (ACK)"            ,  0,0x2106,  0,0x2106,  0,0x2106,
    "Ctrl g (BEL)"            ,  0,0x2207,  0,0x2207,  0,0x2207,
    "Ctrl h (Backspace)"      ,  0,0x2308,  0,0x2308,  0,0x2308,
    "Ctrl j (LF)"             ,  0,0x240A,  0,0x240A,  0,0x240A,
    "Ctrl k (VT)"             ,  0,0x250B,  0,0x250B,  0,0x250B,
    "Ctrl l (FF)"             ,  0,0x260C,  0,0x260C,  0,0x260C,
    "Ctrl ;"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl '"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl `"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl Shift"              ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Ctrl \\ (FS)"            ,  0,0x2B1C,  0,0x2B1C,  0,0x2B1C,
    "Ctrl z (SUB)"            ,  0,0x2C1A,  0,0x2C1A,  0,0x2C1A,
    "Ctrl x (CAN)"            ,  0,0x2D18,  0,0x2D18,  0,0x2D18,
    "Ctrl c (ETX)"            ,  0,0x2E03,  0,0x2E03,  0,0x2E03,
    "Ctrl v (SYN)"            ,  0,0x2F16,  0,0x2F16,  0,0x2F16,
    "Ctrl b (STX)"            ,  0,0x3002,  0,0x3002,  0,0x3002,
    "Ctrl n (SO)"             ,  0,0x310E,  0,0x310E,  0,0x310E,
    "Ctrl m (CR)"             ,  0,0x320D,  0,0x320D,  0,0x320D,
    "Ctrl ,"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl ."                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl /"                  ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl Gray *"             ,  2,0xFFFF,  2,0xFFFF,  0,0x9600,
    "Ctrl Alt"                ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Ctrl Space"              ,  0,0x3920,  0,0x3920,  0,0x3920,
    "Ctrl Caps Lock"          ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl F1"                 ,  0,0x5E00,  0,0x5E00,  0,0x5E00,
    "Ctrl F2"                 ,  0,0x5F00,  0,0x5F00,  0,0x5F00,
    "Ctrl F3"                 ,  0,0x6000,  0,0x6000,  0,0x6000,
    "Ctrl F4"                 ,  0,0x6100,  0,0x6100,  0,0x6100,
    "Ctrl F5"                 ,  0,0x6200,  0,0x6200,  0,0x6200,
    "Ctrl F6"                 ,  0,0x6300,  0,0x6300,  0,0x6300,
    "Ctrl F7"                 ,  0,0x6400,  0,0x6400,  0,0x6400,
    "Ctrl F8"                 ,  0,0x6500,  0,0x6500,  0,0x6500,
    "Ctrl F9"                 ,  0,0x6600,  0,0x6600,  0,0x6600,
    "Ctrl F10"                ,  0,0x6700,  0,0x6700,  0,0x6700,
    "Ctrl F11"                ,  1,0xFFFF,  2,0xFFFF,  0,0x8900,
    "Ctrl F12"                ,  1,0xFFFF,  2,0xFFFF,  0,0x8A00,
    "Ctrl Num Lock"           ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl Scroll Lock"        ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl White Home"         ,  0,0x7700,  0,0x7700,  0,0x7700,
    "Ctrl White Up Arrow"     ,  2,0xFFFF,  2,0xFFFF,  0,0x8D00,
    "Ctrl White PgUp"         ,  0,0x8400,  0,0x8400,  0,0x8400,
    "Ctrl Gray -"             ,  2,0xFFFF,  2,0xFFFF,  0,0x8E00,
    "Ctrl White Left Arrow"   ,  0,0x7300,  0,0x7300,  0,0x7300,
    "Ctrl 5 (number pad)"     ,  2,0xFFFF,  2,0xFFFF,  0,0x8F00,
    "Ctrl White Right Arrow"  ,  0,0x7400,  0,0x7400,  0,0x7400,
    "Ctrl Gray +"             ,  2,0xFFFF,  2,0xFFFF,  0,0x9000,
    "Ctrl White End"          ,  0,0x7500,  0,0x7500,  0,0x7500,
    "Ctrl White Down Arrow"   ,  2,0xFFFF,  2,0xFFFF,  0,0x9100,
    "Ctrl White PgDn"         ,  0,0x7600,  0,0x7600,  0,0x7600,
    "Ctrl White Ins"          ,  2,0xFFFF,  2,0xFFFF,  0,0x9200,
    "Ctrl White Del"          ,  2,0xFFFF,  2,0xFFFF,  0,0x9300,
    "Ctrl SysReq"             ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Ctrl Key 45"             ,  1,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Ctrl Enter (number pad)" ,  1,0xFFFF,  0,0x1C0A,  0,0xE00A,
    "Ctrl / (number pad)"     ,  1,0xFFFF,  2,0xFFFF,  0,0x9500,
    "Ctrl PrtSc"              ,  1,0xFFFF,  0,0x7200,  0,0x7200,
    "Ctrl Break"              ,  1,0xFFFF,  0,0x0000,  0,0x0000,
    "Ctrl Gray Home"          ,  1,0xFFFF,  0,0x7700,  0,0x77E0,
    "Ctrl Gray Up Arrow"      ,  1,0xFFFF,  2,0xFFFF,  0,0x8DE0,
    "Ctrl Gray Page Up"       ,  1,0xFFFF,  0,0x8400,  0,0x84E0,
    "Ctrl Gray Left Arrow"    ,  1,0xFFFF,  0,0x7300,  0,0x73E0,
    "Ctrl Gray Right Arrow"   ,  1,0xFFFF,  0,0x7400,  0,0x74E0,
    "Ctrl Gray End"           ,  1,0xFFFF,  0,0x7500,  0,0x75E0,
    "Ctrl Gray Down Arrow"    ,  1,0xFFFF,  2,0xFFFF,  0,0x91E0,
    "Ctrl Gray Page Down"     ,  1,0xFFFF,  0,0x7600,  0,0x76E0,
    "Ctrl Gray Insert"        ,  1,0xFFFF,  2,0xFFFF,  0,0x92E0,
    "Ctrl Gray Delete"        ,  1,0xFFFF,  2,0xFFFF,  0,0x93E0,
    "Alt Esc"                 ,  2,0xFFFF,  2,0xFFFF,  0,0x0100,
    "Alt 1"                   ,  0,0x7800,  0,0x7800,  0,0x7800,
    "Alt 2"                   ,  0,0x7900,  0,0x7900,  0,0x7900,
    "Alt 3"                   ,  0,0x7A00,  0,0x7A00,  0,0x7A00,
    "Alt 4"                   ,  0,0x7B00,  0,0x7B00,  0,0x7B00,
    "Alt 5"                   ,  0,0x7C00,  0,0x7C00,  0,0x7C00,
    "Alt 6"                   ,  0,0x7D00,  0,0x7D00,  0,0x7D00,
    "Alt 7"                   ,  0,0x7E00,  0,0x7E00,  0,0x7E00,
    "Alt 8"                   ,  0,0x7F00,  0,0x7F00,  0,0x7F00,
    "Alt 9"                   ,  0,0x8000,  0,0x8000,  0,0x8000,
    "Alt 0"                   ,  0,0x8100,  0,0x8100,  0,0x8100,
    "Alt -"                   ,  0,0x8200,  0,0x8200,  0,0x8200,
    "Alt ="                   ,  0,0x8300,  0,0x8300,  0,0x8300,
    "Alt Backspace"           ,  2,0xFFFF,  2,0xFFFF,  0,0x0E00,
    "Alt Tab"                 ,  2,0xFFFF,  2,0xFFFF,  0,0xA500,
    "Alt q"                   ,  0,0x1000,  0,0x1000,  0,0x1000,
    "Alt w"                   ,  0,0x1100,  0,0x1100,  0,0x1100,
    "Alt e"                   ,  0,0x1200,  0,0x1200,  0,0x1200,
    "Alt r"                   ,  0,0x1300,  0,0x1300,  0,0x1300,
    "Alt t"                   ,  0,0x1400,  0,0x1400,  0,0x1400,
    "Alt y"                   ,  0,0x1500,  0,0x1500,  0,0x1500,
    "Alt u"                   ,  0,0x1600,  0,0x1600,  0,0x1600,
    "Alt i"                   ,  0,0x1700,  0,0x1700,  0,0x1700,
    "Alt o"                   ,  0,0x1800,  0,0x1800,  0,0x1800,
    "Alt p"                   ,  0,0x1900,  0,0x1900,  0,0x1900,
    "Alt ["                   ,  2,0xFFFF,  2,0xFFFF,  0,0x1A00,
    "Alt ]"                   ,  2,0xFFFF,  2,0xFFFF,  0,0x1B00,
    "Alt Enter"               ,  2,0xFFFF,  2,0xFFFF,  0,0x1C00,
    "Alt Ctrl"                ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt a"                   ,  0,0x1E00,  0,0x1E00,  0,0x1E00,
    "Alt s"                   ,  0,0x1F00,  0,0x1F00,  0,0x1F00,
    "Alt d"                   ,  0,0x2000,  0,0x2000,  0,0x2000,
    "Alt f"                   ,  0,0x2100,  0,0x2100,  0,0x2100,
    "Alt g"                   ,  0,0x2200,  0,0x2200,  0,0x2200,
    "Alt h"                   ,  0,0x2300,  0,0x2300,  0,0x2300,
    "Alt j"                   ,  0,0x2400,  0,0x2400,  0,0x2400,
    "Alt k"                   ,  0,0x2500,  0,0x2500,  0,0x2500,
    "Alt l"                   ,  0,0x2600,  0,0x2600,  0,0x2600,
    "Alt ;"                   ,  2,0xFFFF,  2,0xFFFF,  0,0x2700,
    "Alt '"                   ,  2,0xFFFF,  2,0xFFFF,  0,0x2800,
    "Alt `"                   ,  2,0xFFFF,  2,0xFFFF,  0,0x2900,
    "Alt Shift"               ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt \\"                  ,  2,0xFFFF,  2,0xFFFF,  0,0x2B00,
    "Alt z"                   ,  0,0x2C00,  0,0x2C00,  0,0x2C00,
    "Alt x"                   ,  0,0x2D00,  0,0x2D00,  0,0x2D00,
    "Alt c"                   ,  0,0x2E00,  0,0x2E00,  0,0x2E00,
    "Alt v"                   ,  0,0x2F00,  0,0x2F00,  0,0x2F00,
    "Alt b"                   ,  0,0x3000,  0,0x3000,  0,0x3000,
    "Alt n"                   ,  0,0x3100,  0,0x3100,  0,0x3100,
    "Alt m"                   ,  0,0x3200,  0,0x3200,  0,0x3200,
    "Alt ,"                   ,  2,0xFFFF,  2,0xFFFF,  0,0x3300,
    "Alt ."                   ,  2,0xFFFF,  2,0xFFFF,  0,0x3400,
    "Alt /"                   ,  2,0xFFFF,  2,0xFFFF,  0,0x3500,
    "Alt Gray *"              ,  2,0xFFFF,  2,0xFFFF,  0,0x3700,
    "Alt Space"               ,  0,0x3920,  0,0x3920,  0,0x3920,
    "Alt Caps Lock"           ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt F1"                  ,  0,0x6800,  0,0x6800,  0,0x6800,
    "Alt F2"                  ,  0,0x6900,  0,0x6900,  0,0x6900,
    "Alt F3"                  ,  0,0x6A00,  0,0x6A00,  0,0x6A00,
    "Alt F4"                  ,  0,0x6B00,  0,0x6B00,  0,0x6B00,
    "Alt F5"                  ,  0,0x6C00,  0,0x6C00,  0,0x6C00,
    "Alt F6"                  ,  0,0x6D00,  0,0x6D00,  0,0x6D00,
    "Alt F7"                  ,  0,0x6E00,  0,0x6E00,  0,0x6E00,
    "Alt F8"                  ,  0,0x6F00,  0,0x6F00,  0,0x6F00,
    "Alt F9"                  ,  0,0x7000,  0,0x7000,  0,0x7000,
    "Alt F10"                 ,  0,0x7100,  0,0x7100,  0,0x7100,
    "Alt F11"                 ,  1,0xFFFF,  2,0xFFFF,  0,0x8B00,
    "Alt F12"                 ,  1,0xFFFF,  2,0xFFFF,  0,0x8C00,
    "Alt Num Lock"            ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt Scroll Lock"         ,  4,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt Gray -"              ,  2,0xFFFF,  2,0xFFFF,  0,0x4A00,
    "Alt Gray +"              ,  2,0xFFFF,  2,0xFFFF,  0,0x4E00,
    "Alt 7 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 8 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 9 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 4 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 5 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 6 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 1 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 2 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt 3 (number pad)"      ,  8,0xFFFF,  8,0xFFFF,  8,0xFFFF,
    "Alt Del"                 ,  2,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Alt SysReq"              ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt Key 45"              ,  1,0xFFFF,  2,0xFFFF,  2,0xFFFF,
    "Alt Enter (number pad)"  ,  1,0xFFFF,  2,0xFFFF,  0,0xA600,
    "Alt / (number pad)"      ,  2,0xFFFF,  2,0xFFFF,  0,0xA400,
    "Alt PrtSc"               ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt Pause"               ,  1,0xFFFF,  4,0xFFFF,  4,0xFFFF,
    "Alt Gray Home"           ,  1,0xFFFF,  2,0xFFFF,  0,0x9700,
    "Alt Gray Up Arrow"       ,  1,0xFFFF,  2,0xFFFF,  0,0x9800,
    "Alt Gray Page Up"        ,  1,0xFFFF,  2,0xFFFF,  0,0x9900,
    "Alt Gray Left Arrow"     ,  1,0xFFFF,  2,0xFFFF,  0,0x9B00,
    "Alt Gray Right Arrow"    ,  1,0xFFFF,  2,0xFFFF,  0,0x9D00,
    "Alt Gray End"            ,  1,0xFFFF,  2,0xFFFF,  0,0x9F00,
    "Alt Gray Down Arrow"     ,  1,0xFFFF,  2,0xFFFF,  0,0xA000,
    "Alt Gray Page Down"      ,  1,0xFFFF,  2,0xFFFF,  0,0xA100,
    "Alt Gray Insert"         ,  1,0xFFFF,  2,0xFFFF,  0,0xA200,
    "Alt Gray Delete"         ,  1,0xFFFF,  2,0xFFFF,  0,0xA300,
    "Key undefined"           ,  0,0x0000,  0,0x0000,  0,0x0000,
};

#endif

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
//ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
//³ INIFILE.CPP
//ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
#define ERC_FILENOTFOUND          2401
#define ERC_ACCESSDENIED          2402
#define ERC_OPENFAILED            2403
#define ERC_DELETEFAILED          2404
#define ERC_RENAMEFAILED          2410
#define ERC_SECTIONNOTFOUND       2405
#define ERC_LABELNOTFOUND         2406
#define ERC_INILABELONLY          2407
#define ERC_CREATETEMPFILEFAILED  2408


class tIniFile
{
   // Note that the Read methods open and DO NOT close the file.
   // The Write methods and the destructor close the file if it is open.
   // The Exists, HasReadAccess & HasWriteAccess methods do not open the file.
   public:
      tIniFile ( char *pFilename );
      ~tIniFile ();
      int   Exists         ( void );
      int   HasReadAccess  ( void );
      int   HasWriteAccess ( void );
      int   Write          ( char *Section, char *Label, char *pData );
      int   Writef         ( char *Section, char *Label, char *pFormat, ... );
      long  ReadLong       ( char *Section, char *Label, long DefaultValue );
      int   Read           ( char *Section, char *Label, char *pDataRet, int *pcbDataRet );
      void  Close          ( void );

   private:
      char  pIniFilename[_MAX_PATH];
      FILE *fIniFile;
      int   OpenForReading;

      char *SkipWhiteSpaces    ( char *p );
      int   IsComment          ( char *p );
      int   IsSection          ( char *p, char *Section );
      char *IsLabel            ( char *p, char *Label );
      void  fputs_lf           ( char *string, FILE *stream );
      void  fputs_section      ( char *Section, FILE *stream  );
      void  fputs_label        ( char *Label, char *pData, FILE *stream  );
};

char *IniFileLastErrorMessage ( void );

/*ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿*/
/*³ MENU.CPP                                                ³*/
/*ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ*/
#define ENTER         0x0D
#define ESCAPE        0x1B
#define TAB           0x09
#define BACKTAB      -0x0F
#define PGUP         -0x49
#define PGDN         -0x51
#define UP           -0x48
#define DOWN         -0x50
#define HOME         -0x47
#define END          -0x4F
#define LEFT         -0x4B
#define RIGHT        -0x4D

#define F1           -0x3B
#define F2           -0x3C
#define F3           -0x3D
#define F4           -0x3E
#define F5           -0x3F
#define F6           -0x40
#define F7           -0x41
#define F8           -0x42
#define F9           -0x43
#define F10          -0x44

#define SHIFT_F1     -0x54
#define SHIFT_F2     -0x55
#define SHIFT_F3     -0x56
#define SHIFT_F4     -0x57
#define SHIFT_F5     -0x58
#define SHIFT_F6     -0x59
#define SHIFT_F7     -0x5A
#define SHIFT_F8     -0x5B
#define SHIFT_F9     -0x5C
#define SHIFT_F10    -0x5D

#define CTRL_F1      -0x5E
#define CTRL_F2      -0x5F


#pragma pack(1)

typedef struct
{
   char     Description[81];
   char     pItemData[256];  // Geoff, Tue 11-Mar-1997, 15:49:37
} MENU_OPTION;
/*
typedef struct
{
   char          Heading[81];
   char          BackgroundCharacter[2];
   unsigned char BackgroundColour;
   unsigned char BorderColour;
   unsigned char NormalColour;
   unsigned char HighlightColour;
   unsigned char ShadowColour;
   int           NumberOfItems;
   char          pMenuData[256];
} MENU_REC;
*/
#pragma pack()

class tMenu
{
   public:
      tMenu ( char *Heading, int NumberOfItems );
      ~tMenu ();
      void InitialiseData ( void );
      int  MenuProcess   ( int InitialItemNumber, int QuitKey, int EditKey );
      void MenuSetItem  ( int ItemNumber, char *Description );
      int  MaxItems ( void );
      virtual void ExtraPaint ( void ) {};

      char          _Heading[81];
      int           _SelectedItemNumber;
      unsigned char _BackgroundCharacter[2];
      unsigned char _BackgroundColour;
      unsigned char _BorderColour;
      unsigned char _NormalColour;
      unsigned char _HighlightColour;
      unsigned char _ShadowColour;
      int           _NumberOfItems;
      char          _pMenuData[256];
      MENU_OPTION  *_pItem;


   private:
      int          _MaxItems;
      int          _Debugging;

      void Debug           ( void );
      int  menu_choice     ( int QuitKey, int EditKey );
      void menu_background ( void );
      void menu_paint      ( void );
      void menu_header     ( void );
      int  menu_width      ( void );
      char ExtractHotkey   ( char *Description, int RemoveHotkeyFromString );
};
#endif
