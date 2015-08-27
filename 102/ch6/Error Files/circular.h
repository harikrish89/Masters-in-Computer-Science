/*******************************************************/
/* circular.h                                              */
/*                                                     */
/*******************************************************/
#ifndef _list
#define _list
#include "globals.h"
typedef struct node { generic_ptr *datapointer; struct node *next; } node;
typedef node *list;

extern list list_iterator(list L, list lastreturn);
extern status traverse(list L, status (*p_func_f) () );
extern status allocate_node( list *p_L, generic_ptr data);
extern void free_node(list *p_L);
extern status init_list(list *p_L);
extern bool empty_list(list L);
extern status insert(list *p_L, generic_ptr data);
extern status append(list *p_L, generic_ptr data);
extern status delete(list *p_L, generic_ptr *p_data);

#endif
