#ifndef _circular
#define _circular
#include "globals.h"

typedef struct node node , *list ; 

struct node{
	generic_ptr datapointer ; 
	list next ; 
};

extern status init_circ_list(list *p_L);
extern bool empty_circ_list(list L);
extern status circ_insert(list *p_L , generic_ptr  data );
extern status circ_append(list *p_L , generic_ptr data) ; 
extern status circ_delete(list *p_L , generic_ptr *p_data);
extern status circ_delete_node(list *p_L ,list node );
extern list circ_list_iterator(list L, list lastreturn ) ; 
extern status circ_traverse(list L, status (*p_func_f)() ) ;
#endif
