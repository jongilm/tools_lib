/* Version 1.05 (March 88) */
/***************************************************************************/
/*                                BUFFERS                                  */
/*                                ÄÄÄÄÄÄÄ                                  */
/* Here are the interrupt input buffers for each of the serial ports. To   */
/* save memory buffers are only allocated for COM1 and COM2. If you want   */
/* to use additional ports you will have to set up characters arrays for   */
/* each additional port you wish to use. Alternatively you could malloc a  */
/* buffer of suitable size and install its address in serbuf[]. see the    */
/* notes below, and refer to the appropriate section in your manual.       */
/***************************************************************************/

/* IMPORTANT ---> Set for FOUR serial ports only */

/* HERE ARE THE BUFFERS themselves, increase the 1st array index by one for  */
/* each additional port in the system, don't forget to increase _numports.   */
/* Each buffer will take up IBUFSIZE of your static data segment. Static     */
/* arrays are used to facilitate memory residence if required. You may remove*/
/* the definition below and set up the buffers from the heap by assigning the*/
/* pointer returned by malloc(IBUFSIZE) to the appropiate serbuf[] pointer   */

#ifndef COMB_H
	#define COMB_H 1

#ifndef SERIAL_H
        #include "comms.h"
#endif
BYTE _i_sbuf[4][IBUFSIZE];

/* An array of pointers to buffers, done this way so that sopen() can check */
/* if a buffer has been set up for that port when opening it for interrupts */
/* for each additional port in the system you should set up an input buffer */
/* either in the array above or by a call to malloc, and place its address  */
/* in the appropriate place in the array below.                             */

BYTE *serbuf[8] = {
		  _i_sbuf[0],		/* com1 */
		  _i_sbuf[1],		/* com2 */
		  _i_sbuf[2],		/* com3 */
		  _i_sbuf[3],		/* com4 */
		  NULL,			/* com5 */
		  NULL,			/* com6 */
		  NULL,			/* com7 */
		  NULL,			/* com8 */
		};

/* ----------------------END OF BUFFER DEFINITION------------------------ */
#endif


