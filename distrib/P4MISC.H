
/* p4misc.h (c)Copyright Sequiter Software Inc., 1987-1990.  All rights reserved.

   Declarations for non-standard C runtime library functions.
*/

/* Default locking is 'DO_LOCKING' (call to 'locking' rather than 'lock') 
   which is MSC. */

#ifndef HC386
#ifndef ZORTECH
#ifndef TURBO
#ifndef __WATCOMC__
#ifndef UNIX
#ifndef MSC
#define MSC
#endif
#endif
#endif
#endif
#endif
#endif

#ifdef GERMAN
#define LANGUAGE
#endif

#ifdef LANGUAGE
#define NO_SIZE_T
#endif

#ifdef HC386
char *fcvt( double, int, int *, int * ) ;
#define near _Near
#define far  _Far
#define huge _Huge
#define IS_386
#define int386 int86
#endif

#ifdef TURBO
#define DO_LOCK
#define CPP
#endif

#ifdef ZORTECH
#define NO_LOCK
#define CPP
#define size_t unsigned
#endif

#ifdef __WATCOMC__
#define NO_LOCK
#define IS_386
#endif

#ifdef UNIX
extern  long  filelength() ;
extern  char *fcvt() ;
extern  long  lseek() ;
#define DO_ERRNO
#define NO_POW
#define NO_SPAWNL
#define NO_FILELENGTH
#define NO_KBHIT
#define NO_MEMMOVE
#define NO_REMOVE
#define NO_RENAME
#define NO_STRUPR
#define NO_STRLWR
#define NO_STRNICMP
#define NO_SIZE_T
#define PORTABLE
#define VARARGS
#endif

#ifdef NO_TEST
#define NO_SIZE_T
#define NO_CHSIZE
#define NO_LOCK
#define NO_MEMMOVE
#define NO_POW
#define NO_SPAWNL
#define NO_FILELENGTH
#define NO_REMOVE
#define NO_RENAME
#define NO_STRLWR
#define NO_STRUPR
#define NO_STRNICMP
#define LANGUAGE
#define PORTABLE
#endif

#ifndef DO_LOCK
#ifndef NO_LOCK
#define DO_LOCKING
#endif
#endif

#ifdef NO_SIZE_T
typedef unsigned   size_t ;
#endif

#ifdef DO_CHSIZE
int  chsize( int, long ) ;
#endif

#ifdef NO_FILELENGTH
long  filelength(int) ;
#endif

#ifdef NO_MEMMOVE
void *memmove( void *, void *, size_t ) ;
#endif

#ifdef NO_REMOVE
int remove( char * ) ;
#endif

#ifdef NO_STRNICMP
int  strnicmp( char *, char *, size_t ) ;
#endif

#ifdef NO_STRUPR
char *strlwr( char * ) ;
#endif

#ifdef NO_STRUPR
char *strupr( char * ) ;
#endif

#ifdef LANGUAGE
extern int v4map[256] ;
int u4memcmp( unsigned char *, unsigned char *, size_t ) ;
#endif
