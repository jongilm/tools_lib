/********** START OF SERIAL.H ****************/
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 SERIAL.H                                     ³
³                                 ÄÄÄÄÄÄÄÄ                                     ³
³               Structure header file for Zortech Comms Library                ³
³       (c)1987 Zortech Ltd, All rights reserved : Written by Dave Mansell     ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/

/* VERSION 1.05 */

#ifndef SERIAL_H
	#define SERIAL_H  1             /* prevent multiple includes */

#ifdef DLC
	#define __ZTC__ 1
#endif

#ifndef __ZTC__
	#ifdef ZC
		#define __ZTC__ 0x0205
	#endif
#endif

/***************** START OF COMDEF.H *******************/
#ifndef COMDEF_H
	#define COMDEF_H 1            /* prevent multiple includes */

#ifdef DLC
	#define __ZTC__ 1
#endif

#ifndef __ZTC__
	#ifdef ZC
		#define __ZTC__ 0x0205
	#endif
#endif

#ifndef NULL
	#include <stdio.h>
#endif

/* default interrupt buffer size */

#define IBUFSIZE  1024         /* (1k buffer, maximum 64k) */

/* macros for timed functions */

#define SEC *18                 /* eg. pause(10SEC);   - pause 10 seconds */
#define MS  /50                 /* eg. pause(200MS);   - pause 200 milliseconds */
				/* or pause(5SEC+500MS);  - pause 5.5 seconds */

/* Constants for set- functions */

#define OFF 0
#define ON  1

/* Error values returned in comerr by sopen() */

#define BAD_ARG  1		/* Character other than 'i' or 'p' passed */
#define BAD_COM  2		/* device number outside the permitted range */
#define IS_OPEN  3		/* device is already open...*/
#define NO_DEV   4		/* the requested device (port) is not fitted */

/* Error values returned by a_sputc() or a_sgetc*/
/* a_sputc returns OK (0) if no errors */

#define USER_CAN 1
#define TIMEOUT -1
#define RETRY -2

/* ASCII control code definitions (Only major codes included) */

#define	  NUL	0x00
#define	  SOH	0x01
#define	  STX	0x02
#define	  ETX	0x03
#define	  EOT	0x04
#define	  ENQ	0x05
#define	  ACK	0x06
#define	  BEL	0x07
#define	  BS	0x08
#define	  TAB	0x09
#define	  LF	0x0a
#define	  FF	0x0c
#define	  CR	0x0d
#define	  XON	0x11
#define	  XOFF	0x13
#define	  NAK	0x15
#define	  CAN	0x18
#define	  ESC	0x1b

#define	  DEL	0x7f
#define	  SP	0x20

/* UART registers - `base' is held in port structure */

#define RXR base			/* Receiver buffer register	*/
#define TXR base			/* Transmitter buffer register	*/
#define IER base+1			/* Interrupt enable register	*/
#define IIR base+2			/* Interrupt identity register	*/
#define LCR base+3			/* Line control register	*/
#define MCR base+4			/* Modem control register	*/
#define LSR base+5			/* Line status register		*/
#define MSR base+6			/* Modem status register	*/
#define BAUDL base			/* LSB baud rate register	*/
#define BAUDH base+1			/* MSB baud rate rgister	*/


/* Masks for various UART registers */

#define DLAB 0x80			/* mask for DLAB bit in LCR	*/
					/* when set allows access to	*/
					/* BAUDL and BAUDH....		*/

#define RX   0x01
#define TX   0x20
#define DTR  0x01
#define RTS  0x02
#define OUT1 0x04
#define OUT2 0x08
#define CTS  0x10
#define DSR  0x20
#define RI   0x40
#define CD   0x80

#define BRK  0x40

#define IRX 0x01
#define ITX 0x02
#define ILS 0x04
#define IMS 0x08


/*****************************************************************************/
/* maximum no (each) of input and output format structures which can be      */
/* saved at any one time using save_form()                                   */
/*****************************************************************************/

#define IO_MAX 8

/*****************************************************************************
 *                            Error definitions                              *
 *****************************************************************************/

#define OK      0                       /* All OK folks!!   */
#define ERROR  -1                       /* General error    */
#define NOT_SUPP -2                     /* function 'not supported' */
/* file transfer errors */
#define ERR_U   1                       /* User cancelled   */
#define ERR_F   2                       /* File open error  */
#define ERR_FR  3                       /* File read error  */
#define ERR_FW  4                       /* File write error */
#define ERR_FC  5                       /* File close error */
#define ERR_N   6                       /* Transfer error   */
#define ERR_R   7                       /* To many retries  */
#define ERR_E   8                       /* To many errors   */
#define ERR_T   9                       /* Timeout error    */
#define ERR_EOF 10                      /* RX EOF error     */
#define ERR_X   11                      /* TX EOF error     */
#define ERR_EOT 12                      /* end of transfer error (multifile) */
#define ERR_S   13                      /* Sync. error      */
#define ERR_C   14                      /* Remote cancelled */
#define ERR_IN  15                      /* Init not acknowledged */
#define ERR_NAK 16                      /* Packet NAK'ed     */
#define ERR_LP  17                      /* Previous packet received */
#define ERR_PI  18                      /* Packet integrity error */
#define ERR_FH  19                      /* File header error */

/* defines used by setfilter */

#define ALLOW -1
#define STRIP -2

#endif

/***************** END OF COMDEF.H *******************/

/* #define sopen _sopen  Jonnie, Wed 22-Jun-1994, 00:28:34 */

#ifndef __ZTC__
	#ifdef __TURBOC__
		#define pokew poke
		#define peekw peek
		#define inpw inport
		#define outpw outport
	#else
		#define MSC 1
		#define getvect _dos_getvect
		#define setvect _dos_setvect
		#define enable _enable
		#define disable _disable
		#define MK2FP(seg,ofs)	((void far *)(((unsigned long)(seg) << 16) | (ofs)))
		#define peekw(a,b)      (*((int far*)MK2FP((a),(b))))
	#endif
#endif

/***************************************************************************/
/* not strictly needed for ZC v2 since all chars are unsigned but helps    */
/* portability to other compilers which may have signed chars              */
/***************************************************************************/

typedef unsigned char BYTE;

/***************************************************************************/
/* Zortech C converts register variable declarations to automatic, however */
/* static variables are faster to use than automatics, and speed is very   */
/* important in comms. This typedef allows for the use of the fastest      */
/* variable possible on the compiler in use when needed.                   */
/***************************************************************************/

#ifdef __ZTC__
	#define FAST static
#else
	#define FAST register
#endif

/***************************************************************************/
/* Definitions for enum variables used in cookout and cookin structs       */
/* if your compiler does not support enum variables you will need to       */
/* replace the typedefs below with, eg. typedef int FLAG                   */
/* and then explicitly define the values for each variable eg.             */
/* #define FALSE 0      #define TRUE 1   etc.....                          */
/***************************************************************************/

typedef enum {FALSE,TRUE} FLAG;
typedef enum {EOLOFF,LF2CR,CR2LF,EOL2SP,ADDCR,ADDLF,STRCR,STRLF,STREOL} EOL_T;
typedef enum {CASOFF,UPPER,LOWER} CAS;
typedef enum {FULL,HALF} ECHO;
typedef enum {NOHAND,XONXOFF,DTRDSR,RTSCTS} HAND;
typedef enum {IN,OUT} IO;

/* Enum definition used in set_form() */

typedef enum	{
					CONSOLE,
					CASE,
					EOL,
					PASS,
					TOASCII,
					F_CTRL,
					F_CHAR,
					HANDSHAKE,
					DUPLEX,
					ECHO2CRT,
					ECHOBACK,
					TIME_OUT,
					CHAR_DELAY,
					LINE_DELAY,
					LINE_CHAR,
					PAUSE_TIME,
					PAUSE_CHAR,
					DESTRUCT_BS,
					ABORT_KEY
				} IO_CONTROL;

/* defines used for CONSOLE mode in set_form() */
#define RAW 0
#define COOKED 1

/***************************************************************************/
/* Structure definitions for all major structure types used in the library */
/***************************************************************************/

struct s_intr {
#ifdef __ZTC__
		int (*irout)();			/* pointer to int server     */
		unsigned long old_vect;		/* old int vector            */
#else
	#ifdef __TURBOC__
		void interrupt (*irout)();	/* pointer to int server     */
		void interrupt (*old_vect)();	/* old int vector            */
	#else
		void (interrupt far *irout)();	/* pointer to int server     */
		void (interrupt far *old_vect)();/* old int vector            */
	#endif
#endif
		int ports;			/* no of ports currently     */
						/* using this interrupt      */
		BYTE ivno;			/* vector to intercept       */
		BYTE pic_mask;			/* mask for PIC              */
		};

struct	form {
		unsigned baudrate;
		BYTE datalen,parity,stopbits,c_mctrl;
		BYTE inten,mctrl,lctrl;
		unsigned int baudiv;
		};


struct cookout {
	FLAG cooked;			/* raw or cooked mode flag */
	EOL_T  eolmode;			/* how to treat end of lines */
	FLAG t_eol;			/* used in conversion of EOL */
	FLAG passmode;			/* allow keyboard entry of cntrls */
	FLAG t_ctrl;			/* used in conversion to cntrl */
	BYTE ctrl_ch;			/* character to represent control */
	CAS  casemode;			/* case conversion mode */
	HAND handmode;			/* handshake mode to use*/
	ECHO duplex;			/* full/half duplex or echo to console*/
	FLAG crt;			/* echo output to the screen */
	FLAG str7bit;			/* force ascii..strip off bit 7 */
	FLAG strctrl;			/* remove all controls except */ 
	int  num_ctrls;			/* number of permitted controls */
	BYTE good_ctrl[32];		/* list of permitted controls */
	FLAG filter;			/* Activate/deactive character - */
	int filtchar[128];		/* substitution table */
	int  char_tim;			/* delay between characters 0 = off*/
	int  line_tim;			/* delay between lines 0 = off*/
	BYTE line_eol;			/* final eol character to start delay*/
	int fix_tim;			/* fixed delay time - fixed delay */
	char fixch;			/* added if this char is detected*/
	char abortkey;			/* key that will abort wait loops */
};

struct cookin {
	FLAG cooked;			/* raw or cooked mode flag */
	EOL_T  eolmode;			/* how to treat end of lines */
	FLAG eolflag;			/* flag to signal eol in progress */
	BYTE t_eol;			/* temporary eol char storage */
	FLAG crt;			/* send input also to console */
	FLAG passmode;			/* represent controls by 2 char seq */
	BYTE ctrl_ch;			/* character to represent control */
	CAS  casemode;			/* case conversion mode */
	HAND handmode;			/* handshaking mode, ishand is TRUE  */
	FLAG ishand;			/* if suspend signal given to sender */
	ECHO echo;			/* echo back to sender or to console*/
	FLAG str7bit;			/* force ascii..strip off bit 7 */
	FLAG strctrl;			/* remove all controls except */ 
	int  num_ctrls;			/* number of permitted controls */
	BYTE good_ctrl[32];		/* list of permitted controls */
	FLAG filter;			/* Activate/deactive character - */
	int filtchar[128];		/* substitution table */
	int totime;			/* timeout period, 0 = no timeout */
	FLAG dsbs;			/* destructive or non destructive - */
};					/* backspace */

/*######################################################*/
/*    THIS IS IT..... the COM structure definition      */
/*######################################################*/

typedef struct	{
		unsigned base;			/* UART base address     */
		FLAG isopen; 			/* is this com port open */
		FLAG isint;			/* set for interrupts or polled ?*/
		volatile FLAG isxoff;		/* TRUE if Xoff received */
		volatile unsigned modstat;	/* modem register status */
		volatile unsigned linstat;	/* line register status  */
		volatile BYTE *buf;		/* ptr to input buffer */
		volatile BYTE *head;		/* head of buffer */
		volatile BYTE *tail;		/* tail of buffer */
		volatile BYTE *end;		/* end of buffer  */
		volatile unsigned cnt;		/* no of bytes in buffer */

		/* variables up to here are also referenced by intserv.asm */

		struct s_intr *istruc;		/* interrupt structure   */
		struct form *dformat;		/* addr of data format structure */
		struct cookout *ascout;		/* ascii output formatting */
		struct cookin *ascin;		/* ascii input formatting */
		struct modem *modpt;		/* pointer to modem control structure */
		BYTE (*rxbyte)();		/* serial port read function */
		BYTE (*rxstat)();		/* read status function  */
		void (*txbyte)();		/* serial port write function*/
		BYTE (*txstat)();		/* write status function */
		int (*disp)();			/* function used to display chars */
		int (*trans)();			/* file transfer function */
		int (*ftmon)();			/* file transfer monitor */
		int wind;			/* ID for use with Zortech windows */
		int com_no;			/* IBM COM number for this device */
		} COM;

/*****************************************************************************
 *            structure definition for modem control functions               *
 *****************************************************************************/

struct modem	{
		char type[40];		/* description of modem */
		FLAG ishayes;		/* true if modem is Hayes compatible */
		char cmdbuf[41];	/* buffer where commands built */
		char lastnum[40];	/* last number dialed, used by redial*/
		char term[3];		/* normal command terminator */
		char tone[20];		/* tone dial prefix */
		char pulse[20];		/* pulse dial prefix */
		int dial;		/* whether pulse or tone dial */
		char hangup[20];	/* hangup code */
		char reset[20];		/* reset code */
		char auto_on[20];	/* auto answer on "" = not supported */
		char auto_off[20];	/* auto answer off "" = as above */
		};

/*****************************************************************/
/* High level functions not returning integers are declared here */
/*****************************************************************/

/* com_l.c */

int	raw(COM *, BYTE), _sdplx(COM *, BYTE),
	_set232(COM *, BYTE, unsigned int), _get232(COM *, BYTE),
	_setpoll(COM *), _setint(COM *), _setbaud(COM *, unsigned int);

BYTE	_p_instat(COM *), _p_outstat(COM *), _i_instat(COM *), _p_read(COM *),
	_i_read(COM *);

void	_p_write(COM *, BYTE), _b_write(COM *, BYTE), _svuart(COM *),
	_lduart(COM *), _setbrk(COM *, BYTE, int), _clrint(COM *);

int setbaud(COM *portnum, unsigned level);
int modem(COM *,...);
void pause(unsigned int t50s);
void sputc();
void sputs();
void rsbreak(COM *cp);
void setterm(COM *cp, int (*funcptr)());
void settrans(COM *cp,int (*funcptr)());
void setmon(COM *cp, int (*funcptr)());
COM *sopen(unsigned int devno,char mode);
unsigned sread();
unsigned getbaud(COM *cp);
unsigned upd_crc();
char getparity(COM *cp);

int set_form(COM *cp,IO direction,IO_CONTROL shake,unsigned handtype);
int sclose(COM *cp);
int a_sgetc(COM *cp);
int a_sputc(COM *cp, BYTE c);
int inkey(void);
int transfer(COM *cp, char *cmd, char *filename);
int getdatalen(COM *cp);
int getstop(COM *cp);
int setparity(COM *cp, char newpar);
int setdatalen(COM *cp, int choice);
int setstop(COM *cp, int choice);
int xmodem(COM *cp,char *ucmd, char *filename);
int kermit(COM *cp,char *ucmd, char **filenames);
int setrts(COM *sp, unsigned int state);
int setdtr(COM *sp, unsigned int state);
int setout1(COM *sp, unsigned int state);
int setout2(COM *sp, unsigned int state);

int getdtr(COM *cp);
int getrts(COM *cp);
int getdsr(COM *cp);
int getcts(COM *cp);
int getcd(COM *cp);
int getri(COM *cp);
int getout1(COM *cp);
int getout2(COM *cp);


/* macros for two UART functions */

#define setcts setrts				/* since wiring is crossed */
#define setdsr setdtr				/* in dte equipment */

/* external variables in other files */
extern int _numports, comerr;
extern FLAG _fetch;
extern struct modem _hayes;

/* file transfer information variables (SEE FTRANS.C) */
extern unsigned long _nbytes;
extern unsigned _packno, _packex;
extern int _errors, _errno, _sending;
extern char *_fname, *_etype;

#endif

/********** END OF SERIAL.H ****************/

/********** START OF MODEM.H ****************/
/*
ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                 MODEM.H                                      ³
³                                 ÄÄÄÄÄÄÄ                                      ³
³                Modem definitions for Zortech Comms Library                   ³
³       (c)1987 Zortech Ltd, All rights reserved : Written by Dave Mansell     ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
*/
#ifndef MODEM_H
	#define MODEM_H 1               /* prevent multiple includes */

/* Definitions for func arguments in modem function */

#define NEWMOD		0
#define MOD_ID		1
#define DIAL		2
#define REDIAL		3
#define HANGUP		4
#define RESET		5
#define AUTO_ON		6
#define AUTO_OFF	7
#define PULSE		9
#define TONE		10
#define CONFIG		11

#define MOD_H		1

extern struct modem _hayes;	/* default modem structure for hayes compats */
#endif

/********** END OF MODEM.H ****************/

/********** START OF SCREENIO.H ****************/

int setscreen(void);
int endscreen(void);
int mode(void);
void scrclr(COM *sp);
void move(COM *sp, int row, int col);
void cursor(int row, int col);
int curpos(COM *sp);
void home(COM *sp);
void curs_u(COM *sp);
void curs_d(COM *sp);
void curs_r(COM *sp);
void curs_l(COM *sp);
void scrcol(COM *sp, int fg, int bg);
void dispchar(COM *sp, char c);
int readc(void);
void dpage(int page);
void eeol(COM *sp);
void eeop(COM *sp);
void rptch(char c,int n);

/********** END OF SCREENIO.H ****************/
