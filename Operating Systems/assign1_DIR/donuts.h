#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/signal.h>

#define		SEMKEY			(key_t)919650398
#define		SHMKEY			(key_t)919744819
#define		NUMFLAVORS	 	4
#define		NUMSLOTS       	50
#define		NUMSEMIDS	 	3
#define		PROD		 	0
#define		CONSUMER	 	1
#define		OUTPTR		 	2


struct	donut_ring{
	int	flavor  [NUMFLAVORS]  [NUMSLOTS];
	int	outptr  [NUMFLAVORS];
};


extern int p (int, int);
extern int v (int, int);
extern int semsetall (int, int, int);

union semun {
    int val;
    struct semid_ds *buf;
   u_short *array;
 };
