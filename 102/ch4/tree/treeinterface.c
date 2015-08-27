#include "treeprimitives.h"
#include "globals.h"
#include <stdlib.h>

status number_bst_insert (tree *T, int a, int(*p_cmp_func) () )
{

int *data;
int rc;

data = (int*) malloc (sizeof(int));

if (data == NULL ) return ERROR ;
*data = a;

rc = bst_insert (T, (generic_ptr) data, p_cmp_func);

return rc;

}

