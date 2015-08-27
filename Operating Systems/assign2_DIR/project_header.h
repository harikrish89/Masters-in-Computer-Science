/****************donuts.h*************/


#include <sys/signal.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdio.h>

#define         NUMFLAVORS       	4
#define         NUMSLOTS           100
#define         NUMCONSUMERS     	50	
#define	  NUMPRODUCERS	30
#define 	  NoOfDozens		200	

struct  donut_ring {
	int     	flavor [NUMFLAVORS] [NUMSLOTS];
	int     	outptr [NUMFLAVORS];
	int		in_ptr [NUMFLAVORS];
	int		serial [NUMFLAVORS];
};


/**********************************************************************/
/* SIGNAL WAITER, PRODUCER AND CONSUMER THREAD FUNCTIONS              */
/**********************************************************************/

	void	*sig_waiter ( void *arg );
	void	*producer   ( void *arg );
	void	*consumer ( void *arg) ;
	void    sig_handler ( int );

/**********************    END HEADER FILE         *******************/