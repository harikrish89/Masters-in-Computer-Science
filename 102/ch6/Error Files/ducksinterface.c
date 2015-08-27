/***************************************************************/
/* ducksinterface.c                                            */
/***************************************************************/
#include "globals.h"
#include "circular.h"
#include <stdlib.h>
extern status circ_append_duck(list *p_L, int n){
int *p_number;
p_number = (int *) malloc(sizeof (int));
*p_number = n;
return circ_append(p_L, (generic_ptr) p_number);
return OK;
}
extern list getnextduck(list ducks, list nextduck) {
list temp;
temp = circ_list_iterator(ducks, nextduck);
if(temp == NULL ) temp = circ_list_iterator(ducks, temp);
return (temp);
}
