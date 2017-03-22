/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ Jonnie's Btrieve Utilities                                                 ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

#include <process.h>   /* for spawnl and P_WAIT */
#include <stdio.h>     /* for printf and NULL   */
#include <stdlib.h>    /* for errno             */
#include <dos.h>       /* for int86             */
#include <string.h>
#include <errno.h>     /* for ENOMEM and ENOENT */

#include <tools.h>     /* for my btrv defs      */

/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ STANDARD BTRIEVE ERROR ROUTINE  -  Array definition                        ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

char *btrieve_err[] =
{
/* 00 */  "Operation successful",
/* 01 */  "Invalid operation",
/* 02 */  "I/O Error",
/* 03 */  "No open",
/* 04 */  "Key not found",
/* 05 */  "Duplicates error",
/* 06 */  "Invalid key number",
/* 07 */  "Different key number",
/* 08 */  "Invalid positioning",
/* 09 */  "End of file",
/* 10 */  "Modifiable error",
/* 11 */  "Invalid file name",
/* 12 */  "File not found",
/* 13 */  "Extension error",
/* 14 */  "Pre-open error",
/* 15 */  "Pre-image error",
/* 16 */  "Expansion error",
/* 17 */  "Close error",
/* 18 */  "Disk full",
/* 19 */  "Unrecoverable error",
/* 20 */  "Record manager inactive",
/* 21 */  "Key buffer error",
/* 22 */  "Record buffer error",
/* 23 */  "Position block error",
/* 24 */  "Page size error",
/* 25 */  "Create I/O error",
/* 26 */  "Number of keys error",
/* 27 */  "Key position error",
/* 28 */  "Record length error",
/* 29 */  "Key length error",
/* 30 */  "Btrieve file name error",
/* 31 */  "Extend error",
/* 32 */  "Extend I/O error",
/* 33 */  "Undocumented error",
/* 34 */  "Extend name error",
/* 35 */  "Directory error",
/* 36 */  "Transaction error",
/* 37 */  "Begin transaction error",
/* 38 */  "Transaction control file error",
/* 39 */  "End/abort error",
/* 40 */  "Transaction max files error",
/* 41 */  "Transaction operation error",
/* 42 */  "Incomplete accelerated access",
/* 43 */  "Invalid data record address",
/* 44 */  "Null key path",
/* 45 */  "Inconsistant key flags",
/* 46 */  "Access denied",
/* 47 */  "Maximum open files",
/* 48 */  "Invalid alternate sequence definition",
/* 49 */  "Key type error",
/* 50 */  "Owner already set",
/* 51 */  "Invalid owner",
/* 52 */  "Error writing cache",
/* 53 */  "Invalid interface",
/* 54 */  "Variable page error",
/* 55 */  "Undocumented error",
/* 56 */  "Incomplete index",
/* 57 */  "Expanded memory error",
/* 58 */  "Undocumented error",
/* 59 */  "Undocumented error",
/* 60 */  "Undocumented error",
/* 61 */  "Undocumented error",
/* 62 */  "Undocumented error",
/* 63 */  "Undocumented error",
/* 64 */  "Undocumented error",
/* 65 */  "Undocumented error",
/* 66 */  "Undocumented error",
/* 67 */  "Undocumented error",
/* 68 */  "Undocumented error",
/* 69 */  "Undocumented error",
/* 70 */  "Undocumented error",
/* 71 */  "Undocumented error",
/* 72 */  "Undocumented error",
/* 73 */  "Undocumented error",
/* 74 */  "Undocumented error",
/* 75 */  "Undocumented error",
/* 76 */  "Undocumented error",
/* 77 */  "Undocumented error",
/* 78 */  "Undocumented error",
/* 79 */  "Undocumented error",
/* 80 */  "Conflict",
/* 81 */  "Lock error",
/* 82 */  "Lost position",
/* 83 */  "Read outside transaction",
/* 84 */  "Record in use",
/* 85 */  "File in use",
/* 86 */  "File full",
/* 87 */  "Handle full",
/* 88 */  "Mode error",
/* 89 */  "Name error",
/* 90 */  "Device full",
/* 91 */  "Server error",
/* 92 */  "Transaction full",
/* 93 */  "Incompatible lock type",
/* 94 */  "BTRIEVE.EXE not found in current directory",     /* my addition */
/* 95 */  "Insufficient memory to load BTRIEVE.EXE",        /* my addition */
/* 96 */  "Error trying to spawn BTRIEVE.EXE",              /* my addition */
/* 97 */  "Data message too small",
/* 98 */  "Undocumented error",
/* 99 */  "Demo error"
};

int load_btrv(void)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ LOAD_BTRIEVE                                                               ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   int      spawn_result,result;

   spawn_result = spawnl(P_WAIT,"BTRIEVE.EXE","/p:1024"," >nul",NULL);
/* spawn_result = system("BTRIEVE.EXE /p:1024 >nul"); */
   if (spawn_result == -1)
   {
      if (errno == ENOENT)      result = 94;
      else if (errno == ENOMEM) result = 95;
           else                 result = 96;
   }
   else   result = 0;
   return(result);
}

int unload_btrv(void)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ UNLOAD BTRIEVE                                                             ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   int b_error;
   b_error = btrv(B_STOP,NULL,NULL,NULL,NULL,0);
   return(b_error);
}


char *b_err(int err_num, char *err_msg)
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³ STANDARD BTRIEVE ERROR ROUTINE                                             ³
³                                                                            ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
{
   strcpy(err_msg,btrieve_err[err_num]);
   return(err_msg);
}





/*
ÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛ
Û               Novell's Btrieve Interface starts here                        Û
Û                                                                             Û
ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
*/

/*                                                                      */
/*           MicroSoft C Version 3.0 interface to the                   */
/*              Btrieve Record Manager, version 4                       */
/*                                                                      */
/*      Note: if compiling for the "D" or "L" memory model, remove      */
/*            the comments surrounding the following definition.        */
/*                                                                      */
/*                                  LMODEL means 32-bit pointers in use
*/
/*#define LMODEL 1*/


/*         make sure this constant is not commented if
                 compiling for MS windows
#define WINDOWS 1
*/

#define BTR_ERR     20                    /* record manager not started */
#define BTR_INT     0x7B                    /* Btrieve interrupt vector */
#define BTR2_INT    0x2F                 /* multi-user interrupt vector */
#define BTR_VECTOR  BTR_INT * 4                 /* offset for interrupt */
#define BTR_OFFSET  0x33               /* Btrieve offset within segment */
#define VARIABLE_ID 0x6176     /* id for variable length records - 'va' */
#define _2FCODE     0xAB00       /* function code for int 2F to btrieve */

/* ProcId is used for communicating with the Multi Tasking Version of  */
/* Btrieve. It contains the process id returned from BMulti and should */
/* not be changed once it has been set.                                */
/*                                                                     */

static unsigned ProcId = 0;             /* initialize to no process id */
static char MULTI = 0;                /* flag set to true if MultiUser */
static char VSet = 0;           /* flag set to true if checked version */

btrv (int OP, char *POS_BLK, char *DATA_BUF, int *DATA_LEN, char *KEY_BUF, int KEY_NUM)
/*
  int  OP;
  char POS_BLK[];
  char DATA_BUF[];
  int  *DATA_LEN;
  char KEY_BUF[];
  int  KEY_NUM;
*/
{
/*struct REGVAL { int AX, BX, CX, DX, SI, DI, CY; } REGS;*/
/*struct SEGREG { short ES, CS, SS, DS; } sregs;*/
union REGS regs;
struct SREGS sregs;



struct BTRIEVE_PARMS      /* structure passed to Btrieve Record Manager */
 {
   char *BUF_OFFSET;                      /* callers data buffer offset */
#ifndef LMODEL
   int   BUF_SEG;                        /* callers data buffer segment */
#endif
   int   BUF_LEN;                              /* length of data buffer */
   char *CUR_OFFSET;                      /* user position block offset */
#ifndef LMODEL
   int   CUR_SEG;                        /* user position block segment */
#endif
   char *FCB_OFFSET;                              /* offset of disk FCB */
#ifndef LMODEL
   int   FCB_SEG;                                /* segment of disk FCB */
#endif
   int   FUNCTION;                                /* requested function */
   char *KEY_OFFSET;                     /* offset of user's key buffer */
#ifndef LMODEL
   int   KEY_SEG;                       /* segment of user's key buffer */
#endif
   char  KEY_LENGTH;                     /* length of user's key buffer */
   char  KEY_NUMBER;                    /* key of reference for request */
   int  *STAT_OFFSET;                          /* offset of status word */
#ifndef LMODEL
   int   STAT_SEG;                            /* segment of status word */
#endif
   int   XFACE_ID;                               /* language identifier */
 } XDATA;

int STAT = 0;                                 /* status of Btrieve call */

/*                                                                      */
/*  Check to see that the Btrieve Record Manager has been started.      */
/*                                                                      */

if (!VSet)                   /* if we don't know version of Btrieve yet */
 {
  VSet = 1;
  regs.x.ax = 0x3000;                              /* check dos version */
#ifdef WINDOWS
  int86 (0x21, &regs, &regs);
#else
  int86x (0x21, &regs, &regs, &sregs);
#endif
  if ((regs.x.ax & 0x00FF) >= 3)           /* if DOS version 3 or later */
   {
    regs.x.ax = _2FCODE;
#ifdef WINDOWS
    int86 (BTR2_INT, &regs, &regs);                /* is bmulti loaded? */
#else
    int86x (BTR2_INT, &regs, &regs, &sregs);
#endif
    MULTI = (char)((regs.x.ax & 0xFF) == 'M'); /* if al is M, bmulti is loaded */
   }
 }

if (!MULTI)
 {                                              /* if bmulti not loaded */
  regs.x.ax = 0x3500 + BTR_INT;
#ifdef WINDOWS
  int86 (0x21, &regs, &regs);         /* check for btrieve being loaded */
#else
  int86x (0x21, &regs, &regs, &sregs);
#endif
  if (regs.x.bx != BTR_OFFSET)
     return (BTR_ERR);
 }

/*  Read segment registers and initialize segment part of addresses to  */
/*  user's data segment.                                                */
/*                                                                      */

segread (&sregs);
#ifndef LMODEL
XDATA.BUF_SEG = XDATA.CUR_SEG = XDATA.FCB_SEG =
  XDATA.KEY_SEG = XDATA.STAT_SEG = sregs.ss;
#endif

/*                                                                      */
/*  Move user parameters to XDATA, the block where Btrieve expects them.*/
/*                                                                      */

XDATA.FUNCTION    = OP;
XDATA.STAT_OFFSET = &STAT;
XDATA.FCB_OFFSET  = POS_BLK;
XDATA.CUR_OFFSET  = POS_BLK + 38;
XDATA.BUF_OFFSET  = DATA_BUF;
XDATA.BUF_LEN     = *DATA_LEN;
XDATA.KEY_OFFSET  = KEY_BUF;
XDATA.KEY_LENGTH  = 255;                 /* use max since we don't know */
XDATA.KEY_NUMBER  = (char)KEY_NUM;
XDATA.XFACE_ID    = VARIABLE_ID;

/*                                                                      */
/*  Make call to the Btrieve Record Manager.                            */
/*                                                                      */

regs.x.dx = (int) &XDATA;      /* parameter block is expected to be in DX */
sregs.ds = sregs.ss;
if (!MULTI)
#ifdef WINDOWS                    /* if bmulti not loaded, call Btrieve */
  int86 (BTR_INT, &regs, &regs);
#else
  int86x (BTR_INT, &regs, &regs, &sregs);
#endif
else
 {                                                       /* call bmulti */
  for (;;) /*while (1)*/
   {
    regs.x.ax = 1;                   /*  assume no proc id obtained yet */
    if ((regs.x.bx = ProcId) != 0)              /* if we have a proc id */
      regs.x.ax = 2;                                /* tell bmulti that */
    regs.x.ax += _2FCODE;
#ifdef WINDOWS
    int86 (BTR2_INT, &regs, &regs);                      /* call bmulti */
#else
    int86x (BTR2_INT, &regs, &regs, &sregs);
#endif
    if ((regs.x.ax & 0x00FF) == 0) break;      /* if call was processed */
#ifndef WINDOWS                                /* by bmulti, leave loop */
    regs.x.ax = 0x0200;     /* if multilink advanced is loaded, it will */
    int86x (0x7F, &regs, &regs, &sregs);    /* it will switch processes */
#endif
   }
  if (ProcId == 0) ProcId = regs.x.bx;
 }

*DATA_LEN = XDATA.BUF_LEN;
return (STAT);                                         /* return status */
}

/*
ÛßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßÛ
Û               Novell's Btrieve Interface ends here                          Û
Û                                                                             Û
ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
*/

/******************************************************************************/
/* GENERAL                                                                    */
/******************************************************************************/
int open_file(char *posblk, char *filename)
{
   return(btrv(B_OPEN,posblk,NULL,NULL,filename,0));
}

int close_file(char *posblk)
{
   return(btrv(B_CLOSE,posblk,NULL,NULL,NULL,0));
}
