/*  w4.h   (c)Copyright Sequiter Software Inc., 1987-1990.  All rights reserved.  */

#ifdef UNIX
   #ifndef CURSES_DONE
      /* This header file is sometimes called 'curses.h': */
      #ifdef H_CURSES
         #include <curses.h>
      #else
         #include <tinfo.h>
      #endif
      #define CURSES_DONE
   #endif

   #define  F_BLUE      A_NORMAL
   #define  F_GREEN     A_NORMAL
   #define  F_RED       A_NORMAL
   #define  F_WHITE     A_NORMAL
   #define  F_INTENSE   A_STANDOUT
   #define  B_BLUE      A_REVERSE
   #define  B_GREEN     A_REVERSE
   #define  B_RED       A_REVERSE
   #define  B_WHITE     A_REVERSE
   #define  B_BLINK     A_BLINK

   #define  SINGLE     11 /*"--||    "*/
   #define  DOUBLE     11 /*"--||    "*/
   #define  DOUBLE_TOP 11 /*"--||    "*/
   #define  PANEL      11 /*"--||    "*/
#else
   #define  F_BLUE      1L
   #define  F_GREEN     2L
   #define  F_RED       4L
   #define  F_CYAN      3L
   #define  F_MAGENTA   5L
   #define  F_YELLOW    6L
   #define  F_WHITE     7L
   #define  F_INTENSE   8L
   #define  B_BLUE   0x10L
   #define  B_GREEN  0x20L
   #define  B_RED    0x40L
   #define  B_WHITE  0x70L
   #define  B_BLINK  0x80L

   #define  SINGLE      1 /*"\xC4\xC4\xB3\xB3\xDA\xBF\xC0\xD9"*/
   #define  DOUBLE      2 /*"\xCD\xCD\xBA\xBA\xC9\xBB\xC8\xBC"*/
   #define  DOUBLE_TOP  3 /*"\xCD\xCD\xB3\xB3\xD5\xB8\xD4\xBE"*/
   #define  PANEL       8 /*"\xDB\xDB\xDB\xDB\xDB\xDB\xDB\xDB"*/
#endif

#define  MAX_GET_WIDTH    258
#define  DEFAULT_DATE    "MMM/DD/YY"

extern  int  v4default_window ;
extern  char v4default_date[] ;

typedef int  ACTION(int) ;

typedef struct cb_window_st
{
   int   next, prev ;

   /* Window Information */
   int   hand ;                  /* A negative handle means use 'screen_ptr' */
   int   start_row ;             /* The windows Origin Row */
   int   start_col ;             /* The windows Origin Column */
   int   row ;                   /* The current Row */
   int   col ;                   /* The current Column */
   int   width ;                 /* Number of characters in screen width */
   int   display_bytes ;         /* Number of bytes on the screen */
   int   height ;                /* The number of rows per page */
   int   active ;                /* True if the window has been displayed */
   int   border_type ;           /* Border Type; 0 if none */
   long  border_attribute ;      /* Border Attribute Character */
   long  attribute ;             /* The Current Attribute */

   #ifdef UNIX
      WINDOW  *win ;
      int      border_offset ;
   #else
      /* Some Extra Curses Definitions */
      char *data ;                  /* A memory copy of the screen data */
      char *old_screen ;            /* The old Screen Information for Popups */
   #endif

   int   title_row ;
   int   title_col ;
   char *title ;
   long  title_attribute ;

   /* Get Information */
   int   first_get ;             /* A Reference to the First Get */
   int   last_get ;              /* A Reference to the Last Get */
   int   release ;               /* True if Gets are Released after Use */
   char  get_delimiter[2] ;
   long  get_attribute ;

   /* Menu Information */
   int   first_menu ;            /* Reference to the first menu item reference */
   int   last_menu ;             /* Reference to the last menu item reference */
   int   start_item ;            /* The starting menu item reference */
   int   horizontal ;            /* True if Horizontal Menu */
   int   item_width ;            /* Column Distance between Menu Items */
   int   key_read ;              /* 1 - Single Char; 2 - String Compare */
   int   ignore_case ;           /* 1 - Ignore case of key */
   long  menu_attribute ;        /* Default normal menu item attribute */
   long  menu_att_active ;       /* Selected menu item attribute */
   int   force_refresh ;         /* Refresh the menu window */
   int   arrow_exit ;            /* TRUE if opposite arrow keys cause
				    'n4activate' to exit */
   int   up_key ;                /* A key to move the menu system up one */
   int   exit_key ;              /* A key to exit the menu system */
   int   return_start ;          /* A range of keys which exit the menu system */
   int   return_end ;
/* 17/04/91 J. GILMORE added code from here */

   int   shadow_type;            /* 1 - None; 2 - Transparent; 3 - Opaque */
   int   shadow_colour;          /* Shadow colour */
   char *shadow_save;            /* Shadow save buffer */
   int   node;                   /* Node number */
   int   message_row;
   int   message_col;


/* 17/04/91 J. GILMORE to here */
}  CB_WINDOW ;


typedef struct menu_st
{
   int    next, prev ;

   int    row ;           /* Specific row of menu item */
   int    col ;           /* Specific column of menu item */

   int    window_ref ;    /* Corresponding window reference number */

   char  *item_ptr ;      /* Menu Item text */
   int    skip_over ;     /* True if the this Menu Item may not be chosen */

   int    key_value ;     /* A key to move to and/or activate the menu item */
   int    key_activate ;  /* Non-zero if the key press activates the menu item */
   int    key_highlight_pos ;  /* -1 if there is no highlighting of special key */

   char  *message ;       /* Displayed with routine 'n4message_do' */
   long   attribute ;     /* Attribute for the menu item (When Not Selected) */

   ACTION *action ;
   ACTION *reaction ;
   int    parm_data ;     /* Parameter for the action/reaction routine. */
   void  *ptr_data ;      /* Pointer data. */
   int    int_data ;      /* Extra integer data. */
}  MENU ;


typedef struct get_st
{
   int   next, prev ;

   int   row ;              /* Row of the entry area */
   int   col ;              /* Column of the entry area */
   int   width_scr ;        /* Width of the Screen */

   int   window_ref ;       /* The Window Reference Number */

   char *picture ;          /* A picture entry template */
   int   num_decimals ;     /* The number of decimals for numeric entries */
   int   upper_convert ;    /* TRUE if complete Upper Case Conversion */

   #ifdef KR
      int (*valid)() ;      /* Routine to call to verify the GET */
      int (*call)() ;       /* Routine to call before the GET */
   #else
      int (*valid)(struct get_st *) ;
      int (*call)(struct get_st *, char *, int) ;
   #endif
   int   call_data ;        /* Some data to be passed to the call routine */
   char  type ;             /* C (Character), D (Date), L (Logical),
                               N (Numeric),   d (double), l (long), i (int) */
   void *data ;             /* Pointer to the Data Buffer */
   int   width_data ;       /* The number of bytes pointed to by 'data' */
   long  attribute ;        /* The attribute to be used for the 'GET' */
   char  delimiter[2] ;     /* Up to two delimeter characters */
   char *message ;          /* A message displayed by 'g4message_do' */

}  GET ;

typedef  int GET_ROUTINE( GET *, char *, int ) ;

GET     *g4alloc( int, int, void *, char ) ;
void     g4( int, int, char * ) ;
long     g4attribute( long ) ;
void     g4bell( void) ;
int      g4bell_set( int ) ;
void     g4call( GET_ROUTINE *, int ) ;
int      g4char( void) ;
void     g4date( int, int, char * ) ;
void     g4delimiter( char * ) ;
void     g4double( int, int, double * ) ;
void     g4display( void) ;
void     g4field( int, int, long ) ;
void     g4int( int, int, int * ) ;
void     g4logical( int, int, int * ) ;
void     g4long( int, int, long * ) ;
int      g4menu( GET *, char *, int ) ;
int      g4menu_help( GET *, char *, int ) ;
void     g4message( char * ) ;
void     g4message_do( char * ) ;
void     g4numeric( int, int, char * ) ;
void     g4picture( char * ) ;
int      g4read( void) ;
int      g4release( int ) ;
void     g4upper( void) ;
void     g4valid( int (*)(GET *)) ; /* g3valid has a routine as a parameter */
void     g4width( int, int ) ;

int      n4( char * ) ;
void     n4action( ACTION * ) ;
int      n4activate( int ) ;
void     n4arrow_exit( void ) ;
void     n4attribute( long, long ) ;
void     n4attribute_item( int, long ) ;
void     n4calc( int, int, int ) ;
int      n4char(void) ;
void     n4char_routine( int (*)(void) ) ;
int      n4sub_menu( int ) ;
void     n4get_calc( int ) ;
void     n4horizontal( void) ;
int      n4int_get( int ) ;
void     n4int_save( int ) ;
int      n4item( int, int, char * ) ;
char    *n4item_text( int ) ;
void     n4item_width( int ) ;
void     n4key( int, int, int ) ;
void     n4key_set( int, int ) ;
void     n4key_special( int, int, int, int ) ;
void     n4lotus( int ) ;
void     n4message( char *) ;
void     n4message_do( char * ) ;
void     n4parm( int ) ;
void    *n4ptr_get( int ) ;
void     n4ptr_save( void * ) ;
void     n4pulldown( int ) ;
void     n4reaction( ACTION * ) ;
void     n4refresh( int ) ;
int      n4search( char * ) ;
int      n4skip_over( int, int ) ;
void     n4start_item( int ) ;

void     w4( int, int, char * ) ;
void     w4activate( int ) ;
long     w4attribute( long ) ;
void     w4border( int, long ) ;
void     w4box( char *, int, int, int, int) ;
void     w4centre( int, char *) ;
void     w4clear( int ) ;
void     w4close( int ) ;
int      w4col( void) ;
void     w4cursor( int, int ) ;
void     w4cursor_size( int, int ) ;
void     w4deactivate( int ) ;
int      w4define( int, int, int, int ) ;
#ifdef MSC
int  cdecl w4display( char *, ... ) ;
#else
int      w4display( char *, ... ) ;
#endif
void     w4double( int, int, double, int, int ) ;
void     w4eject( void) ;
void     w4enlarge(void) ;
void     w4exit(int) ;
void     w4field( int, int, long ) ;
int      w4handle( int ) ;
int      w4height( int ) ;
int      w4init( int, int, int ) ;
void     w4init_undo(void) ;
void     w4int( int, int, int, int ) ;
void     w4long( int,  int, long, int ) ;
void     w4memory( void) ;
void     w4num( int, int, char *, int ) ;
void     w4num_att( int, int, char *, int, long ) ;
void     w4out( char * ) ;
void     w4popup(void) ;
void     w4position( int, int ) ;
void     w4position_set( int, int ) ;
void     w4read( int, int, char *, int ) ;
void     w4read_window( int, char * ) ;
void     w4repeat( int, int, char, int ) ;
int      w4row( void) ;
void     w4scroll( int ) ;
int      w4select( int ) ;
void     w4shrink(void) ;
void     w4title( int, int, char *, long ) ;
int      w4width( int ) ;
void     w4write( int, int, char *, int ) ;
void     w4write_att( int, int, char *, int, long ) ;
void     w4write_window( int, char * ) ;

/* 17/04/91 J. GILMORE added code from here */

void  w4shadow( int shadow_type, int shadow_colour ) ;
void  w4node( int node ) ;
void  w4message_pos( int message_row, int message_col ) ;

int   w4sizeof_scnfile(int node);
int   w4read_scnfile(int node,char *screen_buffer);
int   w4write_scnfile(int node,char *screen_buffer,int scncount);

char *w4border_chars(int bordertype);

void  w4draw_shadow(int y1,int x1,int y2,int x2,int shadow,int colour);
char *w4read_shadow(int y1,int x1,int y2,int x2);
void  w4write_shadow(int y1,int x1,int y2,int x2,char *save_shadow);

/* 17/04/91 J. GILMORE to here */

#ifdef UNIX
   void  w4refresh_off() ;
   void  w4refresh_on() ;
   void  w4refresh(int) ;
#endif
