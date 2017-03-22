
/*  d4base.h   (c)Copyright Sequiter Software Inc., 1987-1990.  All rights reserved. */

#ifdef HC386
   #define NO_HUGE
#endif

#ifdef __WATCOMC__
   #define NO_HUGE
#endif

#ifdef ZORTECH
   #define NO_HUGE
#endif

#ifdef TURBO
   #define NO_HUGE
#endif

#ifdef  NO_HUGE
   #define  H_PTR  *
#else
   #define  H_PTR  huge *
#endif

typedef struct field_st
{
   char    name[11] ;
   char    type ;
   short   width ;
   short   decimals ;
   short   offset ;
}  FIELD ;

typedef struct base_buf_st
{
   int   next,prev ;	  

   long  start_rec ;   /* -1L means none. */
   long  end_rec ;
   int   changed ;     /* TRUE if the buffer has been modified. */
}  BASE_BUF ;


/*   BASE */

typedef struct base_st
{
   int     next ;            /* -1 flags last file in list */
   int     prev ;            /* -1 flags first file in list */

   char    name[64] ;        /* full path file name */
   int     file_hand ;
   long    rec_lock ;        /*  Record Number Locked
                                 0 if no specific record is locked */
   int     file_lock ;       /*  -1 Nothing; 0 Count Bytes; 1 File Locked  */

   long    rec_num  ;        /* The record number. */
   int     bof ;             /* Beginning of File; TRUE or FALSE */
   int     eof ;             /* End of File; TRUE or FALSE */

   int     memo_file ;       /* Handle for Memo File */
   int     memo_size ;       /* The memo file block size */

   /* Buffer Information */
   int     buffer_len ;      /* The first character is the deletion flag */

   char   *old_buf ;         /* The original buffer must be kept for
                                updating index files.  ie  finding the
                                key which needs to be replaced */
   char   *buffer  ;         /* The new_buf memory is allocated with the
                                old_buf memory */
   int     buffer_changed;   /* True if the buffer has been changed since  
                                last written to disk. */

   int     num_fields ;      /* The number of data fields in the database */
   FIELD  *fields ;          /* A pointer to the field information */

   int     index_ref ;       /* A reference to the last index file opened
                                for the database */
   int     current_index ;   /* A reference to the index file actively used */

   int     filter_ref ;      /* The first filter reference for the database */
   int     relate_ref ;      /* The first relation reference for the database */

   int     change_list ;     /* A double linked list of changed buffers. */
   int     buf_may_lend ;    /* True if buffer memory can be temporarily 
                                released for 'packing', 'reindexing', ... */
   int     buf_status ;      /* 0 No Buffer; 1 Buffer Lent; 2 Buffer Avail. */
   long    rec_tot ;         /* Total number of records in all the buffers.*/
   long    rec_per_buf ;     /* Number of records per buffer. */
   long    bytes_per_buf ;   /* Number of bytes per buffer. */
   int     max_bufs ;	     /* The no. of buffer slots allocated for 'bufs'. */
   int     n_bufs ;          /* The no. of buffers currently in use. */
   BASE_BUF   *bufs ;
   char H_PTR  buf_ptr ;     /* Pointer to the start of the data. */

   /* Database Header Information */
   char     version ;        /* 83H with .dbt, 03H without */
   char     yy ;             /* Last Update */
   char     mm ;
   char     dd ;
   long     num_recs ;       
   unsigned short header_len;/* Header Length (Indicates # of fields) */

}  BASE ;

typedef struct key_st
{
   long  file_block ;
   long  rec_num ;
   char  value[ 512 - 3*sizeof(long) ] ;  /* The key size is variable */
}  KEY ;

#ifdef CLIPPER
   #define  BLOCK_SIZE     1024
   #define  MAX_KEY_SIZE    338
#else  
   #define  BLOCK_SIZE      512
   #define  NULL_DATE   1.0E100
   #define  MAX_KEY_SIZE    100
#endif

#define  LOCK_START   1000000000L

int    b4browse( int (*)(void), int (*)(void) ) ;
void   b4call( int (*)(int,int) ) ;
int    b4edit( int (*)(void), int (*)(void) ) ;
void   b4margin(int,int) ;
int    b4quick_browse(void) ;
int    b4quick_edit(void) ;
void   b4verify( int (*)(int) ) ;
double c4atod( char *, int) ;
int    c4atoi( char *, int) ;
long   c4atol( char *, int) ;
char * c4dtoa( double, int, int) ;
int    c4dtok( int, double, char * ) ;
void   c4dt_str( char *, double *) ;
int    c4dt_julian( char *, double *) ;
char * c4dt_format( char *, char *) ;
char * c4dt_unformat( char *, char *) ;
void   c4encode( char *, char *, char *, char *) ;
int    c4julian( int, int, int)       ;
char * c4ltoa( long, char *, int) ;
void   c4trim_n( char *, int ) ;
int    d4append(void) ;
int    d4append_blank(void) ;
void   d4blank(void) ;
int    d4bof(void) ;
int    d4bottom( void ) ;
long   d4buf_init( long, long, long ) ;
int    d4buf_total( long, int, int ) ;
int    d4buf_unit( long ) ;
int    d4create( char *, int, FIELD *, int ) ;
int    d4close( void ) ;
int    d4close_all( void ) ;
int    d4delete( long) ;
int    d4deleted( void ) ;
int    d4eof(void) ;
FIELD *d4fields(void) ;
int    d4flush( int ) ;
int    d4go( long) ;
int    d4init( void ) ;
int    d4init_undo( void ) ;
int    d4initialize( int, int, int, int, long ) ;
int    d4lock( long, int) ;
int    d4lock_all( int, int ) ;
int    d4lock_code( int ) ;
int    d4lock_wait( int ) ;
int    d4locked(long) ;
int    d4lseek( long ) ;
char * d4name(void) ;
int    d4pack( void ) ;
BASE * d4ptr( void ) ;
int    d4recall( long) ;
long   d4recno( void ) ;
long   d4reccount( void ) ;
int    d4ref( char *) ;
int    d4seek( void *) ;
int    d4seek_double( double ) ;
int    d4seek_str( char * ) ;
int    d4seek_int( int ) ;
int    d4select( int ) ;
int    d4skip( long) ;
int    d4top( void ) ;
int    d4unlock( long) ;
int    d4use( char * ) ;
int    d4use_excl( char * ) ;
int    d4write( long) ;
int    d4zap( long, long ) ;
void * e4eval( char *) ;
void * e4exec( char *) ;
int    e4length(void) ;
char * e4string( char * ) ;
char * e4name( char) ;
int    e4parse( char *, char **) ;
char   e4type( void ) ;
char * e4type_str( int) ;
int    e4type_error( char, int, int, int) ;
void * e4vary( char * ) ;
int    f4char( long ) ;
int    f4decimals( long) ;
double f4double( long ) ;
char * f4name( long) ;
int    f4int( long ) ;
long   f4j_ref( int ) ;
long   f4long( long ) ;
int    f4ncpy( long, char *, int ) ;
int    f4num_fields ( void ) ;
char * f4ptr( long) ;
void   f4r_double( long, double ) ;
void   f4r_long( long, long ) ;
void   f4r_int( long, int ) ;
void   f4r_char( long, char ) ;
void   f4r_str( long, char * ) ;
void * f4record( void ) ;
int    f4record_width( void ) ;
long   f4ref( char *) ;
int    f4replace( long, void *) ;
char * f4str( long) ;
int    f4true( long) ;
char   f4type( long) ;
double f4value( long) ;
int    f4width( long) ;
int    i4add( int, char *, long) ;
int    i4bottom( int) ;
long   i4check( int) ;
int    i4close( int) ;
char * i4eval( int) ;
int    i4free( int ) ;
int    i4go( int, char *, long) ;
int    i4index( char *, char *, int, int) ;
int    i4index_filter( char *, char *, int, int, int (*)(void)) ;
KEY *  i4key( int) ;
int    i4lock( int, int) ;
char * i4name( int ) ;
int    i4open( char *) ;
int    i4ref( char *) ;
int    i4reindex( int ) ;
int    i4remove( int, char *, long) ;
int    i4seek( int, void *) ;
int    i4seek_ref( void ) ;
int    i4select( int) ;
long   i4skip( int, long) ;
int    i4top( int) ;
char   i4type( int) ;
int    i4unlock( int) ;
void   i4unselect( void ) ;
long * m4check( long ) ;
int    m4convert( char * ) ;
int    m4edit( long, long, char *, int ) ;
int    m4exist( long ) ;
int    m4read( long, long, char *, int ) ;
int    m4renamed( char * ) ;
int    m4write( long, long, char *, int ) ;
int    m3edit( long, long, char *, int ) ;
int    m3exist( long ) ;
int    m3read( long, long, char *, int ) ;
int    m3write( long, long, char *, int ) ;
int    u4file_first( char *, char * ) ;
int    u4file_next( char * ) ;
int    u4lock( int, long, long, int) ;
int    u4open( char *, int ) ;
int    u4name_char( char ) ;
void   u4name_full( char *, char *, char * ) ;
void   u4name_part( char *, char *, int, int ) ;
void   u4sort() ;
int    u4unlock( int, long, long ) ;
int    x4buffer( long, char *, unsigned int ) ;
void   x4buffer_copy( int, char * ) ;
int    x4blank( void ) ;
int    x4copy( char *, long, int ) ;
int    x4edit( void ) ;
int    x4filter( int (*)(void) ) ;
int    x4filter_pop( void ) ;
void   x4filter_reset( void ) ;
int    x4filter_do( void ) ;
int    x4go( long) ;
int    x4list( void ) ;
int    x4pack( int ) ;
int    x4relate( char *, int, int, long ) ;
int    x4relate_reset( void ) ;
int    x4relate_do( void ) ;
int    x4seek( char *) ;
int    x4seek_double( double ) ;
int    x4seek_str( char * ) ;
int    x4skip( long ) ;
int    x4sort( char *, char *, long, int) ;
int    x4bottom( void ) ;
int    x4top( void ) ;
double x4sum( long ) ;
int    x4insert( long ) ;

#ifdef UNIX
   int   u4error() ;
   void  i4filter() ;
#else
#ifdef MSC
   int    cdecl u4error( int, char *, ... ) ;
#else
   int    u4error( int, char *, ... ) ;
#endif
   void   i4filter( int (*)(void) ) ;
#endif
