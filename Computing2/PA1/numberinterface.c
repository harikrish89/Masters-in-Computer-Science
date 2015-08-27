/********************************************************************/
/******numberinterface.c ********************************************/
/********************************************************************/
#include <stdlib.h>
#include "numberinterface.h"
#include "globals.h"
#include "list.h"
#include "list.c"
status appendanumber(list *p_L, int n)
{
 int *t_ptr;
 if(!empty_list(*p_L))
 {
 append(*p_L, n);
 }
 else
 insert(*p_L, n);
 return OK;
}
status getnextnumber(list *p_L, int *p_n)
{
 if(!empty_list(*p_L))
 {
 traverse(*p_L, *p_n);
 }
 else
 return ERROR;
 return OK;
}
