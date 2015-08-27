

#include "donuts.h"

 int shmid, semid[3];
 
 int dozens = 10;

int main(int argc, char *argv[])
{
  int output1 [NUMFLAVORS][12];

  int p1,j,r,s,t;
  struct donut_ring *shared_ring;
  struct timeval randtime, present;
  
   
   if((shmid = shmget (SHMKEY, sizeof(struct donut_ring),
		            IPC_CREAT | 0600)) == -1){
	perror("Memory not shared: ");
	exit(1);
   }
   
   
   if((shared_ring = (struct donut_ring *) 
		   shmat (shmid, (const void *)0, 0)) == -1){
	perror("Memory not shared: ");
	exit(2);
   }
  
  
   for(p1=0; p1<NUMSEMIDS; p1++){
      if ((semid[p1] = semget (SEMKEY+p1, NUMFLAVORS, 
			 IPC_CREAT | 0600)) == -1){
	perror("semaphore not alloted: ");
	exit(3);
      }
   }
  /*******Time function ***********/
   
   gettimeofday (&randtime, (struct timezone *)0);
   
  /******microsecond component ***************/
   
   unsigned short xsub1[3];
   xsub1[0] = (ushort) randtime.tv_usec;
   xsub1[1] = (ushort)(randtime.tv_usec >> 16);
   xsub1[2] = (ushort)(getpid());

   int maximumlength = 12;
	
   for( p1 = 0; p1 < dozens; p1++ ) {

      for(r = 0; r < NUMFLAVORS; r++){
       for(s = 0; s < maximumlength; s++){
         output1 [r][s] = 0;
       }
      }



      for( t = 0; t < maximumlength; t++ ) {
         
		 j=nrand48(xsub1) & 3;
		
		 int operation = shared_ring->outptr[j];
		 
         p(semid[CONSUMER], j);
         p(semid[OUTPTR], j);
		 
		 
         output1[j][t] = shared_ring->flavor[j][operation];
         operation = operation++; 
		 operation = operation % NUMSLOTS;

         v(semid[OUTPTR], j);
         v(semid[PROD], j);
      }
     gettimeofday (&present, (struct timezone *)0);
     
	 printf("Consumer_process_ID : %d, Time : %d , dozen#  %d\n",
                getpid(), (ushort)present.tv_usec, p1+1);
     
     printf("\n   plain   jelly   coconut   honey-dip\n");
      
	  for(s = 0; s < maximumlength; s++){
       for(r = 0; r < NUMFLAVORS; r++){
         if(output1[r][s] != 0)
             printf("   %3d", output1[r][s]);
         else
             printf("         ");
       }
       printf("\n");
      }
	
      printf("\n");
   }
}
