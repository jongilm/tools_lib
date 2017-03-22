/*  h4memory.h   (c)Copyright Sequiter Software Inc., 1987-1990.  All rights reserved.  

    if  first_free >= num_unit then
    no space is left
*/

typedef struct memory_st
{
   int     first_free ;
   int     num_unit ;
   int     len_unit ;
   int     add_unit ;
}  MEMORY, *MEMORYPTR ;

typedef struct unit_st
{
   int  next ;
   int  prev ;
}  UNIT ;
