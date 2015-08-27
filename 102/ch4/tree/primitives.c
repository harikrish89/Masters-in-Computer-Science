#include "globals.h"
#include "queue.h"
#include "treeprimitives.h"
#include <stdlib.h>
#include <stdio.h>

status allocate_tree_node (tree *p_T, generic_ptr data)
{

tree T = (tree) malloc (sizeof(tree_node));
if ( T == NULL) return ERROR;

*p_T = T;
DATA(T) = data;
LEFT (T) = NULL ;
RIGHT (T) = NULL ;
return OK ;

}

status free_tree_node (tree *p_T)
{
free (*p_T);
*p_T = NULL ;
}

status init_tree (tree *p_T)
{
*p_T = NULL:
return OK;
}

bool empty_tree (tree T)
{
return (T == NULL) ? TRUE : FAlSE ;
}

status make_root (tree *p_T, generic_ptr data, tree left, tree right)
{
if (empty_tree (*p_T) == FALSE ) 
return ERROR ;

if (allocate_tree_node(p_T, data) == ERROR ) 
return ERROR;

LEFT (*p_T) = left;
RIGHT (*p_T) = right;
return OK;
}

void destroy_tree (tree *p_T, void (*p_func_f) () )
{
if (empty_tree (*p_T) == FALSE )
{
destroy_tree (&LEFT(*p_T), p_func_f);
destroy_tree (&RIGHT(*p_T), p_func_f);

if (p_func_f != NULL) (*p_func_f) (DATA (*p_T));
free_tree_node (p_T);
}
}

static status preorder_traverse ( tree T, status (*p_func_f) () )
{
status rc;
if (empty_tree(T) == TRUE )
return OK;

rc = (*p_func_f)(DATA(T));

if (rc == OK) 
rc = preorder_traverse (LEFT(T), p_func_f);

if (rc == OK)
rc = preorder_traverse (RIGHT(T), p_func_f);
return rc;
}

static status inorder_traverse (tree T, status (*p_func-f) () )
{
status rc ;

if (empty_tree(T) == TRUE) 
return OK;
rc = inorder_traverse (LEFT(T), p_func_f);

if( rc == OK)
rc = (*p_func_f)(DATA (T));

if( rc == OK)
rc = inorder_traverse (RIGHT(T), p_func_f);
return rc;
}

static status postorder_traverse(tree T, status(*p_func_f) ())
{
status rc;

if(empty_tree(T) == TRUE)
return OK;
rc = postorder_traverse(LEFT(T), p_func_f);

if (rc == OK)
rc = postorder_traverse(RIGHT(T), p_func_f);

if (rc == OK) 
rc = (*p_func_f)(DATA (T));
return rc ;
}


static status level_traverse(tree T, status (*p_func_f) ())
{
status rc;
queue q;
tree temp;
generic_ptr data;

if (empty_tree(T) == TRUE )
return OK;

init_queue (&q);

qadd (&q, (generic_ptr)T);

while (!empty_queue(&q))
{
qremove(&q, (generic_ptr*) (&Temp));
T=Temp;

(*p_func_f)(DATA (T));

if(!empty_tree(LEFT(T))) 
qadd (&q, (generic_ptr) LEFT(T));

if(!empty_tree(RIGHT(T)))
qadd(&q, (generic_ptr) RIGHT(T));
}
return OK;
}

extern status traverse_tree (tree T, status (*p_func_f) (), ORDER order)
{
switch (order)
{
case PREORDER: return preorder_traverse (T, p_func_f);
case INORDER: return inorder_traverse (T, p_func_f);
case POSTORDER: return postorder_traverse (T, p_func_f);
case LEVEL: return level_traverse(T, p_func_f);
}
return ERROR;
}

extern status bst_insert(tree *p_T, generic_ptr data, int (*p_cmp_func) ())
{
tree T = NULL;
int cmp;

if (empty_tree (*p_T) == TRUE)
{
if (make_root (&T, data, NULL, NULL) == ERROR)
return ERROR;

*p_T = T;
return OK;
}

else {
cmp = ((*p_cmp_func)(data, DATA(*p_T)));

if(cmp < 0) 
return bst_insert(&LEFT (*p_T), data, p_cmp_func);

if(cmp > 0)
return bst_insert(&RIGHT(*p_T), data, p_cmp_func);

if(cmp == 0)
return OK;
/* Will not insert the same value twice. */
}
}


