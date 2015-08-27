/***********************************************************/
/* queue.c                                                 */
/***********************************************************/

#include "queue.h"
#include "globals.h"
#include "list.h"

extern status init_queue(queue *p_Q) 
{ 
return init_list(p_Q);
}

extern bool empty_queue(queue *p_Q)
{
return empty_list(*p_Q);
}

extern status qadd(queue *p_Q, generic_ptr data)
{
return append(p_Q,data);
}

extern status qremove(queue *p_Q, generic_ptr*p_data)
{
return delete(p_Q,p_data);
}

