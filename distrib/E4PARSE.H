
/*  e4parse.h  (c)Copyright Sequiter Software Inc., 1987-1990.	All rights reserved. */

/* This first section specifies the breakdown of the functions 
   in file 'e4functi.c'.
*/

/* Codes for Immediate Data in Compile String */
#define  I_FIELD_STR        0
#define  I_FIELD_LOG        1
#define  I_FIELD_DATE_D     2
#define  I_FIELD_DATE_S     3
#define  I_FIELD_NUM_D      4
#define  I_FIELD_NUM_S      5
#define  I_FIELD_MEMO       6

#define  LAST_I_FIELD       5

#define  I_STRING           6
#define  I_DOUBLE           7

#define  LAST_IMMEDIATE     7


#define  FIRST_LOG          8  /* Range of Logical Operators */
#define  LAST_LOG     	   12

#define  FIRST_OPERATOR    13  /* Range of Other Operators */
#define  LAST_OPERATOR     48

#define  FIRST_FUNCTION    49  /* Start of the List of Functions */

/* Additional Definitions */

#define   MAX_PARMS         3
#define   PARM_STACK_SIZE  20 

typedef struct e4parm_st
{
   union
   {
      void   *v ;
      char   *c ;
      int    *i ;
      long   *l ;
      double *d ;
   }  p ;	   /* Pointer to the parameter */
   int    type ;   /* The type of the parameter */
   int    len ;	   /* The number of bytes in the parameter */
   #ifdef PORTABLE
      int    extra_len ;  /* Extra bytes for alignment purposes. */
   #endif
}  E4PARM ;

typedef void E4FUNC( E4PARM *) ;

typedef struct e4functions_st
{
   int      code ;
   int      name_len ;
   char     name[12] ;
   #ifdef KR
      void     (*function_ptr)() ;
   #else
      void     (*function_ptr)( E4PARM *) ;
   #endif
   int      priority ;
   int      return_type ;
   int      num_parms ;	     /* 0 - No Return;  -1 First Parm. Specifies # */
   int      type[MAX_PARMS] ;        /* type[0] is the return type */
}  E4FUNCTIONS ;

#define OPERATOR_LEN  128

/* Integer Codes for Function/Operator parameters and returns */
#define   T_NUM_STR     ((int) 'n')
#define   T_NUM_DOUB    ((int) 'N')
#define   T_DATE_DOUB   ((int) 'd')
#define   T_DATE_STR    ((int) 'D')
#define   T_STR         ((int) 'C')
#define   T_LOG         ((int) 'L')     /* Used for Logical */
/*  #define   T_LONG        ((int) 'l')   Not Currently Used */

/* Quasi Operators */
#define  Q_NO_FUNCTION  -2
#define  Q_L_BRACKET    -3
#define  Q_R_BRACKET    -4
#define  Q_COMMA        -5

void  e4true(E4PARM *) ;
void  e4false(E4PARM *) ;
void  e4add(E4PARM *) ;
void  e4add(E4PARM *) ;
void  e4concatenate(E4PARM *) ;
void  e4sub(E4PARM *) ;
void  e4concat_two(E4PARM *) ;
void  e4multiply(E4PARM *) ;
void  e4divide(E4PARM *) ;
void  e4power(E4PARM *) ;
void  e4not_equal(E4PARM *) ;
void  e4equal(E4PARM *) ;
void  e4greater(E4PARM *) ;
void  e4less(E4PARM *) ;
void  e4greater_eq(E4PARM *) ;
void  e4less_eq(E4PARM *) ;
void  e4not(E4PARM *) ;
void  e4or(E4PARM *) ;
void  e4and(E4PARM *) ;
void  e4ctod(E4PARM *) ;
void  e4dtoc(E4PARM *) ;
void  e4dtos(E4PARM *) ;
void  e4stod(E4PARM *) ;
void  e4date(E4PARM *) ;
void  e4year(E4PARM *) ;
void  e4month(E4PARM *) ;
void  e4day(E4PARM *) ;
void  e4del(E4PARM *) ;
void  e4deleted(E4PARM *) ;
void  e4iif(E4PARM *) ;
void  e4reccount(E4PARM *) ;
void  e4recno(E4PARM *) ;
void  e4str(E4PARM *) ;
void  e4substr(E4PARM *) ;
void  e4time(E4PARM *) ;
void  e4upper(E4PARM *) ;
void  e4val(E4PARM *) ;
void  e4contain(E4PARM *) ;

extern int  e4return_len( E4PARM *, int ) ;
