/*********************************************************************
|              WWW.PICCIRCUIT.COM  (C) Copyright 2009 		         |   
**********************************************************************
| This source code may only be used with PICCIRCUIT products only.   | 
| If you distribute the source code, the source code must have this  | 
| entire copyright and disclaimer notice. No other use, reproduction | 
| or distribution is permitted without written permission.			 | 
**********************************************************************
|	Program: delay.h												 |
|	Version: 1.1													 |
|   Function: DelayUs(x)	Delay x microseconds 					 |
|   		  DelayMs(x)	Delay x milliseconds	 				 |
|	For DelayUs(x): 20MHz: x>=10									 |
|					16MHz: x>=50									 |
|					8MHz:  x>=100									 |
|					4MHz:  x>=200									 |
|-------------------------------------------------------------------*/

#define	MHZ	*1000L			/* number of kHz in a MHz */
#define	KHZ	*1			/* number of kHz in a kHz */

#if	XTAL_FREQ == 20MHZ

	#define	DelayUs(x)	{ unsigned int _dcnt; \
		_dcnt = (x)/3; \
		while(--_dcnt != 0) \
	continue; }

#elif XTAL_FREQ == 16MHZ

	#define	DelayUs(x)	{ unsigned int _dcnt; \
		_dcnt = (x)/3.75; \
		while(--_dcnt != 0) \
	continue; }

#elif XTAL_FREQ == 8MHZ

	#define	DelayUs(x)	{ unsigned int _dcnt; \
		_dcnt = (x)/7.5; \
		while(--_dcnt != 0) \
	continue; }

#elif XTAL_FREQ == 4MHZ

	#define	DelayUs(x)	{ unsigned int _dcnt; \
		_dcnt = (x)/15; \
		while(--_dcnt != 0) \
	continue; }

#else
	#error Please define XTAL_FREQ - 20MHZ/16MHZ/8MHZ/4MHZ
#endif

void DelayMs(unsigned int cnt){
	do {
		DelayUs(1000);
	} while(--cnt);
}