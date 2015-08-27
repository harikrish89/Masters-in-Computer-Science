#include <stdio.h>
#include <stdlib.h>
#include "circular.h"
#include "globals.h"

status init_circ_list(p_L)
list *p_L;
{
 /*
  * Initialize *p_L by setting the list pointer to NULL.
  * Always return OK (a different implementation
  * may allow errors to occur).
  */
 *p_L = NULL;
return OK;
}

bool empty_circ_list(L)
list L;
{
  /*
   * Return TRUE if L is an empty list, FALSE otherwise.
   */
  return (L == NULL) ? TRUE : FALSE;
}

status circ_insert(p_L, data )
list *p_L;
generic_ptr data;
{
list L;
if (allocate_node(&L, data) == ERROR)
return ERROR;

if(empty_circ_list(*p_L) == TRUE)
{
NEXT(L) = L;
*p_L = L;
}
else 
{
NEXT(L) = NEXT(*p_L);
NEXT(*p_L) = L;
}
return OK;
}

status allocate_node(p_L, data)
list *p_L;
generic_ptr data;
{
list L = (list) malloc(sizeof(node));
if (L == NULL)
return ERROR;

*p_L = L;
DATA(L) = data;
NEXT(L) = NULL;
return OK;
}

void free_node(p_L)
list *p_L;
{
free(*p_L);
*p_L = NULL;
}

status circ_append(p_L, data)
list *p_L;
generic_ptr data;
{
if (circ_insert(p_L, data) == ERROR)
return ERROR;
*p_L = NEXT(*p_L);
return OK;
}

status circ_delete_node(list *p_L,list node)
{
if(empty_circ_list(*p_L) == TRUE)
return ERROR;
if(node == NEXT(node))
*p_L = NULL;
else
{
  list L;
  for (L = NEXT(*p_L); L != *p_L && NEXT(L) != node; L = NEXT(L));

if (NEXT(L) != node)
return ERROR;
NEXT(L) = NEXT(node);

if(node == *p_L)
*p_L = L;

}
free_node(&node);
return OK;
}

status circ_delete(p_L, p_data)
list *p_L;
generic_ptr *p_data;
{
if (empty_circ_list(*p_L))
return ERROR;

*p_data = data(NEXT(*p_L));
 return (circ_delete_node (p_L, *p_L));
}





list circ_list_iterator(L, lastreturn)
list L;
list lastreturn;
{
 /* Return each item of L in turn. Return NULL
  * after the last item has been returned.
  * lastreturn is the value that was returned last.
  * If lastreturn is NULL, start at the beginning of L.
  */
if(lastreturn == NULL)
return (L) ? NEXT(L) : NULL;
else
return (lastreturn == L) ? NULL : NEXT(lastreturn);
}

int circ_length(L)
list L;
{
 /* 
  * Return the number of nodes in L.
  */
list lastreturn;
int length;
length = 0;
lastreturn = NULL;
while((lastreturn = circ_list_iterator(L, lastreturn)) != NULL)
length++;
return length;
}

list nth_node(L, number)
list L;
int number;
{
/* 
 * Return the number'th node of L. The first node is 1.
 * To get the last node, number should be -1. If there aren't 
 * enough nodes in the list, return NULL.
 */
list tmp;
if(empty_circ_list(L) == TRUE)
return NULL;
if(number == -1)
return L;
tmp = L;
do {
tmp = NEXT(tmp);
number--;
}
while (number > 0 && tmp != L);
return (number !=0) ? NULL : tmp;
}

status circ_traverse(L, p_func_f)
list L;
status (*p_func_f)();
{
/* 
 * Call p_func_f() with the DATA field of each node in L.
 * If p_func_f() ever returns ERROR, this function returns ERROR.
 */
list tmp;

if(empty_circ_list(L) == TRUE)
return OK;
tmp = L;
do {
tmp = NEXT(tmp);
if((*p_func_f) (DATA(tmp)) == ERROR)
return ERROR;
}
while (tmp != L);
return OK;
}


