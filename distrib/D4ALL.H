/*  d4all.h   (c)Copyright Sequiter Software Inc., 1987-1990.  All rights reserved. */

#include "d4base.h"

typedef  int  I4FILTER(void) ;

/*    INDEX   */

typedef struct index_st
{
   int     next ;
   int     prev ;

   char    name[64] ;
   int     file_hand ;
   int     lock ;

   int     base_ref ;        /* The corresponding reference number for the
                                database */
   char   *compile ;         /* A pseudo compiled version of the expression */

   int     block_ref ;       /* A reference to the current block of the index file */

   int     block_first ;     /* The first used memory block */
   int     block_last ;      /* The last used memory block */
   int     block_num ;	     /* The number of blocks in the buffer chain */
   int     block_max ;	     /* The maximum number of blocks in the chain */

   char    i_type ;          /* 'C' - Character;  'N' - Numeric; 'D' -Date */
   I4FILTER *filter ;        /* A filter routine used by 'i4reindex' and 'i4add' */

   /* Index File Header Information */

   #ifdef CLIPPER
      short          old_version ;
      long           virtual_eof ;   /* The next available file block */

      short          sign ;
      short          version ;
      long           root ;          /* Root Block */
      long           eof ;           /* First Free Block Pointer */
      short          group_len ;     /* Key Length + 2*sizeof(long) */
      short          key_len ;       /* Key Length */
      short          key_dec ;       /* Number of Decimals in Key */
      short          keys_max ;      /* Maximum # of keys per block;  <= 100 */
      short          keys_half ;     /* Maximum # of keys per half block */
      char           expression[256];/* The index expression corresponding to the
                                        database. */
      short          unique   ;      /* TRUE if Unique */
   #else
      long           old_version ;


      long           root ;
      long           eof  ;
      long           version ;
      short          key_len  ;
      short          keys_max ;      /* Maximum # of keys per block */
      short          int_or_date ;   /* TRUE (1) if Numeric or Date Key */
      short          group_len ;     /* key_len plus 8 increased to a multiple of 2 */
      short          dummy ;                      
      short          unique ;        /* TRUE if Unique */
      char           expression[256];/* The index expression corresponding to the
                                        database. */
   #endif
}  INDEX ;


typedef struct block_st
{
   int   next ;
   int   prev ;

   int   wrt ;         /* Must Write the Block */
   long  file_block ;
   int   key_on     ;  /* The current key.  Starts at 0.  -1 means none.
                          num_keys+1 for key_on indicates EOF */

   /* Index File Block Format:

      Number of Keys

      Repeat for Each Key in Block -
         Block Pointer  (Starting from 0)
         Record Number
         Key
   */

   short num_keys ;

   #ifdef CLIPPER
      short pointers[511] ;
   #else
      short dummy ;
      KEY   key ;
   #endif

}  BLOCK ;

typedef  struct  h4buf_st
{
   long   len ;
   long   avail ;
   long   sub_avail ;
   char   H_PTR  buf_ptr ;
}  H4BUF ;

typedef struct  x4filter_st
{
   int	next,prev ;
   #ifdef KR
      int (*filter_routine)() ;
   #else
      int (*filter_routine)(void) ;
   #endif
}  X4FILTER ;

typedef struct  x4relate_st
{
   int	  next,prev ;

   int	  control_base_ref ;
   char  *compile ;

   /* Identify the database and index file to be repositioned */
   int	  base_ref, index_ref ;
   long   miss_code ;
}  X4RELATE ;

int    b4add( int, KEY *) ;
int    b4down( int, int) ;
int    b4get( int ) ;
int    b4leaf( int) ;
int    b4remove( int) ;
int    b4search( int, char *) ;
long   b4skip( int, long) ;
int    b4up( int) ;
int    b4write( int, int) ;
int    c4clip( char *, int) ;
void H_PTR d4buf_alloc( long, unsigned) ;
long   d4buf_avail(void) ;
void   d4buf_avail_set(void) ;
int    d4buf_calc( void ) ;
int    d4buf_clear( BASE *, int ) ;
int    d4buf_clear_after(int) ;
int    d4buf_clear_all(void) ;
int    d4buf_clear_base( int ) ;
int    d4buf_clear_lend(int) ;
int    d4buf_flush( BASE *, int ) ;
char H_PTR d4buf_read_pos( BASE *, long ) ;
void   d4buf_reset(void) ;
void   d4buf_restore_status( H4BUF * ) ;
void   d4buf_return( void ) ;
void   d4buf_save_status( H4BUF * ) ;
void   d4buf_sub_set(void) ;
char H_PTR d4buf_write_pos( BASE *, long ) ;
int    d4changed( BASE * ) ;
int    d4go_error(long) ;
int    d4init_memory( int, int, int, int ) ;
int    d4read(long) ;
int    d4seek_key( int, void * ) ;
int    h4add( char **, int, int, int) ;
char * h4alloc( int ) ;
char * h4alloc_try( int ) ;
int    h4create( char **, int, int, int) ;
int    h4first( char **, int ) ;
int    h4free( char **, int) ;
void   h4free_memory( void * ) ;
void   h4free_chain( char **, int ) ;
int    h4free_check( int ) ;
void   h4free_test( void * ) ;
int    h4get( char **, int) ;
int    h4move( char **, int, int, int ) ;
char * h4range( int, int ) ;
int    h4remove( char **, int) ;
int    i4filter_check( int ) ;
int    i4keycmp( int, char *, char * ) ;
void H_PTR u4huge_alloc( long ) ;
int    u4huge_cmp( unsigned char H_PTR, unsigned char H_PTR, unsigned);
void   u4huge_cpy( void H_PTR, void H_PTR, long ) ;
void   u4huge_free( void H_PTR ) ;
void H_PTR u4huge_norm( void H_PTR ) ;
long   u4huge_read( int, char H_PTR, long ) ;
void   u4huge_set( char H_PTR, int, long ) ;
int    u4huge_write(int, char H_PTR, long ) ;
char  *u4lower( char * ) ;
int    u4ptr_equal( void *, void * ) ;
int    u4read( char H_PTR, unsigned int ) ;
int    u4read_init( int, char H_PTR, unsigned int, long ) ;
int    u4remove( char * ) ;
void   u4sort_quick( char H_PTR H_PTR, long, int ) ;
int    u4sort_add( char *, char *) ;
long   u4sort_bytes( int, long, int ) ;
void   u4sort_free( void ) ;
int    u4sort_get( char H_PTR * ) ;
int    u4sort_init( long, int, int ) ;
int    u4temp_create( char * ) ;
int    u4tolower( int ) ;
int    u4toupper( int ) ;
char  *u4upper( char * ) ;
int    u4write( char H_PTR, unsigned int ) ;
int    u4write_init( int, char H_PTR, unsigned int, long ) ;
int    u4write_flush( void ) ;
