#include "list.h"
#include "globals.h"
#include <stdio.h>
#include <stdlib.h>

status allocate_node(list *p_L , generic_ptr data)
{
  list L = (list) malloc(sizeof(node));
  if (L == NULL) 
  return ERROR;
  *p_L = L;
  DATA(L) = data;
  NEXT(L) = NULL;
  return ( OK );
}

extern void free_node(list *p_L) 
{
  free(*p_L);
  *p_L = NULL;
}

status init_circ_list(list *p_L)
{
  *p_L = NULL ; 
  return ( OK ) ; 
}

bool empty_circ_list(list L)
{
  return (L == NULL ) ? TRUE : FALSE ; 
}

status circ_insert(list *p_L , generic_ptr  data )
{
  list L ; 
  if(allocate_node(&L,data) == ERROR) 
    return ERROR ; 
  if(empty_circ_list(*p_L) == TRUE)
{
    NEXT(L) = L ; 
    *p_L = L ; 
}
  else
  {
    NEXT(L) = NEXT(*p_L) ; 
    NEXT(*p_L) = L ; 
  } 
  return OK ; 
}
 
extern status circ_append(list *p_L , generic_ptr data)
{ 
 if(circ_insert(p_L,data) == ERROR)
 return ERROR ; 
 *p_L = NEXT(*p_L) ; 
 return OK ; 
}
    
extern status circ_delete(list *p_L , generic_ptr *p_data)
{
 if(empty_circ_list(*p_L))
 return ERROR ; 
 *p_data = (generic_ptr)DATA(NEXT(*p_L)) ;
  return circ_delete_node(p_L , *p_L) ; 
}

extern status circ_delete_node(list *p_L , list node )
{
  list L ; 
  if(empty_circ_list(*p_L) == TRUE  )
    return ERROR ; 
  if(node == NEXT(node))
    *p_L = NULL ; 
  else
{ 
    for(L = NEXT(*p_L) ; L != *p_L && NEXT(L) != node  ; L = NEXT(L) ) ; 
    if (NEXT(L) != node) 
      return ERROR ; 
    NEXT(L) = NEXT(node) ; 
    if( node == *p_L ) 
      *p_L = L ; 
}
  free_node(&node) ; 
  return OK ; 
}

extern status circ_traverse( list L, status (*p_func_f)() ){
  list tmp;
  if( empty_circ_list(L) == TRUE ) return OK;
  tmp = L;
  do{
    tmp = NEXT(tmp);
    if( (*p_func_f)(DATA(tmp)) == ERROR) return ERROR;
  } while( tmp != L );
  
  return OK;
}
 
extern list circ_list_iterator( list L, list lastreturn )
{
  if( lastreturn == NULL ) 
   return (L) ? NEXT(L) : NULL;
  else 
   return (lastreturn == L) ? NULL : NEXT(lastreturn);
}

