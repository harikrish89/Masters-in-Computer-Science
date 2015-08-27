


/**********************************************************************/
/* SIGNAL WAITER, PRODUCER AND CONSUMER THREAD FUNCTIONS              */
/**********************************************************************/

	void	*sig_waiter ( void *arg );
	void	*producer   ( void *arg );
	void	*consumer ( void *arg) ;
	void    sig_handler ( int );



/****************************/
/*      GLOBAL VARIABLES    */
/****************************/

#include "project_header.h"

	struct donut_ring 	shared_ring;
	int			space_count [NUMFLAVORS];		
	int			donut_count [NUMFLAVORS];        
	int			serial [NUMFLAVORS];			
	pthread_mutex_t	prod [NUMFLAVORS];
	pthread_mutex_t	cons [NUMFLAVORS];
	pthread_cond_t	prod_cond [NUMFLAVORS];
	pthread_cond_t	cons_cond [NUMFLAVORS];
	pthread_t		thread_id [NUMCONSUMERS+NUMPRODUCERS], sig_wait_id;


int  main ( int argc, char *argv[] )
{
	int			i, j, k, nsigs;
	struct timeval	randtime, first_time, last_time;
	struct sigaction	new_act;
	int			arg_array[NUMCONSUMERS];
	sigset_t  		all_signals;
	int sigs[] 		={ SIGBUS, SIGSEGV, SIGFPE };
	pthread_attr_t 		thread_attr;
	struct sched_param  sched_struct;
	
	




/**********************************************************************/
/* INITIAL TIMESTAMP VALUE FOR PERFORMANCE MEASURE                    */
/**********************************************************************/

        gettimeofday (&first_time, (struct timezone *) 0 );


        for ( i = 0; i < NUMCONSUMERS ; i++ ) {
			arg_array [i] = i+1;	/** SET ARRAY OF ARGUMENT VALUES **/
        }

/**********************************************************************/
/* GENERAL PTHREAD MUTEX AND CONDITION INIT AND GLOBAL INIT           */
/**********************************************************************/

	for ( i = 0; i < NUMFLAVORS; i++ ) {
		pthread_mutex_init ( &prod [i], NULL );
		pthread_mutex_init ( &cons [i], NULL );
		pthread_cond_init ( &prod_cond [i],  NULL );
		pthread_cond_init ( &cons_cond [i],  NULL );
		shared_ring.outptr [i]		= 0;
		shared_ring.in_ptr [i]		= 0;
		shared_ring.serial [i]		= 0;
		space_count [i]			= NUMSLOTS;
		donut_count [i]			= 0;
	}



/**********************************************************************/
/* SETUP FOR MANAGING THE SIGTERM SIGNAL, BLOCK ALL SIGNALS           */
/**********************************************************************/

	sigfillset (&all_signals );
	nsigs = sizeof ( sigs ) / sizeof ( int );
	for ( i = 0; i < nsigs; i++ ){
       		sigdelset ( &all_signals, sigs [i] );
		}
	sigprocmask ( SIG_BLOCK, &all_signals, NULL );
	sigfillset (&all_signals );
	for( i = 0; i <  nsigs; i++ ) {
      		new_act.sa_handler 	= sig_handler;
      		new_act.sa_mask 	= all_signals;
      		new_act.sa_flags 	= 0;
      		if ( sigaction ( sigs[i], &new_act, NULL ) == -1 ){
         			perror("can't set signals: ");
         			exit(1);
      		}
   	}

	printf ( "just before threads created\n" );


/*********************************************************************/
/* CREATE SIGNAL HANDLER THREAD, PRODUCER AND CONSUMERS              */
/*********************************************************************/

        if ( pthread_create (&sig_wait_id, NULL,
					sig_waiter, NULL) != 0 ){

                printf ( "pthread_create failed " );
                exit ( 3 );
        }

       pthread_attr_init ( &thread_attr );
       pthread_attr_setinheritsched ( &thread_attr,
		PTHREAD_INHERIT_SCHED );

        /*
	sched_struct.sched_priority = PRI_OTHER_MAX;
        pthread_attr_setinheritsched ( &thread_attr,
		PTHREAD_EXPLICIT_SCHED );
        pthread_attr_setschedpolicy ( &thread_attr, SCHED_OTHER );
        pthread_attr_setschedparam ( &thread_attr, &sched_struct );  
        pthread_attr_setscope ( &thread_attr,
		PTHREAD_SCOPE_PROCESS );
	*/



	for(i =0 ;i<NUMPRODUCERS ;i++){
		if ( pthread_create (&thread_id[i], &thread_attr,
						producer, NULL ) != 0 ) {
				printf ( "pthread_create failed " );
				exit ( 3 );
						}
	}

	usleep(100);	

	for ( i = NUMPRODUCERS; i < NUMCONSUMERS + NUMPRODUCERS; i++ ) {
		if ( pthread_create ( &thread_id [i], &thread_attr,
				consumer, ( void * )&arg_array [i - NUMPRODUCERS ]) != 0 ){
			printf ( "pthread_create failed" );
			exit ( 3 );
		}
	}

	printf ( "just after threads created\n" );

/*********************************************************************/
/* WAIT FOR ALL CONSUMERS TO FINISH, SIGNAL WAITER WILL              */
/* NOT FINISH UNLESS A SIGTERM ARRIVES AND WILL THEN EXIT            */
/* THE ENTIRE PROCESS....OTHERWISE MAIN THREAD WILL EXIT             */
/* THE PROCESS WHEN ALL CONSUMERS ARE FINISHED                       */
/*********************************************************************/

	for ( i = NUMPRODUCERS; i < NUMCONSUMERS + NUMPRODUCERS; i++ ) {
             		pthread_join ( thread_id [i], NULL ); 
	}

/*****************************************************************/
/* GET FINAL TIMESTAMP, CALCULATE ELAPSED SEC AND USEC           */
/*****************************************************************/


    gettimeofday (&last_time, ( struct timezone * ) 0 );
        	if ( ( i = last_time.tv_sec - first_time.tv_sec) == 0 )
			j = last_time.tv_usec - first_time.tv_usec;
        	else{
			if ( last_time.tv_usec - first_time.tv_usec < 0 ) {
				i--;
				j = 1000000 + 
				   ( last_time.tv_usec - first_time.tv_usec );
            	} else {
				j = last_time.tv_usec - first_time.tv_usec; }
        	       }
	printf ( "Elapsed consumer time is %d sec and %d usec\n", i, j );

	printf ( "\n\n ALL CONSUMERS FINISHED, KILLING  PROCESS\n\n" );
	exit ( 0 );
}

/*********************************************/
/* INITIAL PART OF PRODUCER.....             */
/*********************************************/

void	*producer ( void *arg )
{
	int		i, j, k;
	
       int 		donuts_count;
	
       unsigned short 	xsub [3];
	struct timeval 	randtime;
	int thr_id = pthread_self ( );
	FILE *producer_file ;
	char producer_id[10];
	char producer_filename[20]="prod_";
	gettimeofday ( &randtime, ( struct timezone * ) 0 );
	xsub [0] = ( ushort 	 ) randtime.tv_usec;
	xsub [1] = ( ushort 	 ) ( randtime.tv_usec >> 16 );
	xsub [2] = ( ushort 	 ) ( pthread_self );

	for ( i = 0; i < ( NUMPRODUCERS ); i++) {
		if ( thr_id == thr_id [i] )  {
				thr_id = i;
				break;
		}
	}

	 sprintf(producer_id, "%d", thr_id) ; 
	 strcat(producer_filename,producer_id);
	if((producer_file = fopen(producer_filename,"a+"))==NULL){
		perror("fopen failed");
		producer_file = stdout;
           	fprintf(stderr, "File not open %s\n", producer_filename);
	}	

	while ( 1 ) 
	{
	  donuts_count = nrand48 ( xsubu ) & 3;
	    
	  pthread_mutex_lock ( &prod [donuts_count] );

      	while ( space_count [donuts_count] == 0 ) 
	{
                pthread_cond_wait ( &prod_cond [donuts_count], &prod [donuts_count] );
      	}


	serial[donuts_count] ++;	

	fprintf(producer_file,"thread %d is producing donuts of type :%d , serial number of donut: %d \n",thr_id,donuts_count,serial[donuts_count]);

	shared_ring.flavor[donuts_count][shared_ring.in_ptr[donuts_count]]=serial[donuts_count];
	
	/** Increasing NUMSLOTS **/		
	shared_ring.in_ptr[donuts_count] = (shared_ring.in_ptr[donuts_count]+1) % NUMSLOTS ;

	/********* Decreasing the space count ********/

	space_count [donuts_count] --; 
	
    	/*****Locking and Unlocking of Mutex*****/

       pthread_mutex_unlock ( &prod [donuts_count] );

	pthread_mutex_lock(&cons [donuts_count]);
	
	donut_count [donuts_count]++;

	pthread_mutex_unlock(&cons [donuts_count]);
	
	pthread_cond_signal(&cons_cond[donuts_count]);
			
	usleep(10000);
	}
	fclose(producer_file);
	return NULL;
}



/*********************************************/
/* Consumer Code    */
/*********************************************/



void    *consumer ( void *arg )
{
	int     i, j, k, m, id ,donuts_count,ptr_output,donut_number;
	unsigned short 	xsub [3];
	struct timeval 	randtime;
       id = *( int * ) arg;
       int printresult;
	int 	linear [NUMFLAVORS];				
	int	total [NUMFLAVORS][12];	
	FILE *consumer_file;
	char file_number[10];
	char consumer_filename[10]="cons_";
	time_t present_time;
	sprintf(file_number, "%d", id) ;    
	strcat(consumer_filename,file_number);
	gettimeofday ( &randtime, ( struct timezone * ) 0 );
	xsub [0] = ( unsigned short )randtime.tv_usec;
	xsub [1] = ( unsigned short ) ( randtime.tv_usec >> 16 );
	xsub [2] = ( unsigned short ) ( pthread_self );
     
	if ((cons_file = fopen(consumer_filename, "a+")) == NULL)
           fprintf(stderr, "Cannot open %s\n", "consumer_file");

	for( i = 0; i < 10; i++ ) 
	{
		for( k = 0; k < NUMFLAVORS; k++)
		{
             		for( m = 0; m < 12; m++)
			{
               		total[k][m] = -1;
                     }
			
		}

		for(k = 0; k < NUMFLAVORS; k++)
		{
			linear [k] = 0;
		}

		for( m = 0; m < 12; m++ ) 
		{
			
		  donuts_count =  nrand48( xsub ) & 3;
		
			pthread_mutex_lock ( &cons [donuts_count] );
			while ( donut_count [donuts_count] == 0 ) 
			{
				pthread_cond_wait ( &cons_cond [donuts_count], &cons [donuts_count] );
			}

		/**********using Donuts*********/

			ptr_outptr = shared_ring.ptr_outptr[donuts_count]; 
			donut_number = shared_ring.flavor[donuts_count][ptr_outptr];
			total[donuts_count][linear[donuts_count]]= donut_number ;
			linear[donuts_count] ++;

			ptr_outptr ++;
			shared_ring.outptr[donuts_count] = ptr_outptr % NUMSLOTS;
			donut_count [donuts_count] --; 

                     /*Locking and Unlocking the Mutex*/
			pthread_mutex_unlock ( &cons [donuts_count] );
        		pthread_mutex_lock(&prod [donuts_count]);
			
			/*increasing the space count*/	
			space_count [donuts_count] ++;
			pthread_mutex_unlock(&prod [donuts_count]);
			
			/*signaling the producer*/
			pthread_cond_signal(&prod_cond[donuts_count]);
			
			}
			 
           		present_time=time(NULL);
			fprintf(consumer_file,"Consumer %d finished collecting dozen#: %d  \n",id,i);
			fprintf(consumer_file,"Consumer thread#: %d , time: %s, Dozen#: %d",id,asctime(localtime(&present_time)),i);
			fprintf(consumer_file,"\t plain \t jelly \t coconut \t honey-dip \t");
			fprintf(consumer_file,"\n");

			printresult=0;
                     for (i = 1; i < NoOfDozens + 1; i++)
                     {
			for( m = 0; m < 12  && printresult <=3 ; m++)
			{
				printresult = 0;
				for(k = 0; k < NUMFLAVORS ; k++)
				{				
 					if(total[k][m] >= 0)
					{
						fprintf(consumer_file,"\t %d \t",total[k][m]);
						printresult = 0;
					}
					else
					{
						fprintf(consumer_file,"\t   \t");
						printresult =printresult+1;
					}				 

				}
				fprintf(consumer_file,"\n");				
			}

}


		/*****************************************************************/
		/* USING A MICRO-SLEEP AFTER EACH DOZEN WILL ALLOW ANOTHER       */
		/* CONSUMER TO START RUNNING, PROVIDING DESIRED RANDOM BEHVIOR   */
		/*****************************************************************/

 	fflush(consumer_file);
         usleep(1000); /* sleep 1 ms */
         }


		fprintf(consumer_file,"Consumer %d finished  collecting all 10 dozen donuts  \n",id);
		fclose(consumer_file);
         return NULL;
}

/***********************************************************/
/* PTHREAD ASYNCH SIGNAL HANDLER ROUTINE...                */
/***********************************************************/

void    *sig_waiter ( void *arg )
{
	sigset_t	sigterm_signal;
	int		signo;

	sigemptyset ( &sigterm_signal );
	sigaddset ( &sigterm_signal, SIGTERM );
	sigaddset ( &sigterm_signal, SIGINT );

	if (sigwait ( &sigterm_signal, & signo)  != 0 ) {
		printf ( "\n  sigwait ( ) failed, exiting \n");
		exit(2);
	}
	printf ( "process going down on SIGNAL (number %d)\n\n", signo );
	exit ( 1 );
	return NULL;
}


/**********************************************************/
/* PTHREAD SYNCH SIGNAL HANDLER ROUTINE...                */
/**********************************************************/

void	sig_handler ( int sig )
{
	pthread_t	signaled_thread_id;
	int		i, thread_index;

	signaled_thread_id = pthread_self ( );
	for ( i = 0; i < (NUMCONSUMERS + NUMPRODUCERS ); i++) {
		if ( signaled_thread_id == thread_id [i] )  {
				thread_index = i;
				break;
		}
	}
	printf ( "\nThread %d took signal # %d, PROCESS HALT\n",
				thread_index, sig );
	exit ( 1 );
}

