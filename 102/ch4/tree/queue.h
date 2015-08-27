/*************************************************************/
/*  queue.h                                                  */
/*************************************************************/
#ifndef _queue
#define _queue

#include "globals.h"
#include "list.h"

typedef list queue;

extern status init_queue(queue *p_Q);
extern bool empty_queue(queue *p_Q);
extern status qadd(queue *p_Q, generic_ptr data);
extern status qremove(queue *p_Q, generic_ptr*p_data);

#endif
