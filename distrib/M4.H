
/* m4.h   (c)Copyright Sequiter Software Inc., 1987-1990. All rights reserved. */

extern  char * m4name(int) ;
extern  int    m4open(int) ;

typedef struct memo_block_st
{
   short minus_one ;
   short start_pos ;
   long  num_chars ;
}  MEMO_BLOCK ;

typedef  struct memo_header_st
{
   long  next_block ;
   long  zero ;
   char  file_name[8] ;
   short zero2 ;
   short x102 ;
   short block_size ;
   short zero3 ;
}  MEMO_HEADER ;


/* For 'm4write' */

#define  CHAIN_SIZE    ((unsigned int)(sizeof(long)*2))

typedef  struct free_area_st
{
   long  next ;       /* The next free block area */
   long  num ;        /* The number of free blocks in the free block area */
   long  to_disk ;    /* TRUE if this information needs to be written to disk */
   long  block_no ;   /* The current block number */
}  FREE_AREA ;

typedef  struct m_st
{
   FREE_AREA  prev ;        /* The previous free area */
   FREE_AREA  cur ;         /* The current free area */

   int        block_size ;  /* The memo block size */
   int        hand ;        /* The file handle */
   int        base_ref ;    /* The database reference number of the memo file */

   int        write_done ;  /* Has the information been written */
   int        free_done ;   /* Has the old information been freed */
}  M ;  /* Memo Write Status Information */

