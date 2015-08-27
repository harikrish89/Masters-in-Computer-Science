#ifndef _primitives
#define _primitives

#include "globals.h"

typedef struct tree_node tree_node, *tree ;

struct tree_node 

{

generic_ptr datapointer;
tree left;
tree right;

};

typedef enum { PREORDER, INORDER, POSTORDER, LEVEL } ORDER;

extern status allocate_tree_node ( tree *p_T , generic_ptr data ) ;
extern status free_tree_node (tree *p_T) ;
extern status init_tree (tree *p_T);
extern bool empty_tree (tree T);
extern status make_root (tree *p_T, generic_ptr data, tree left, tree right );
extern void destroy_tree (tree *p_T, void (*p_func_f) () );
extern status traverse_tree (tree T, status (*p_func_f) (), ORDER order);
extern status bst_insert (tree *p_T, generic_ptr data, int (*p_cmp_func) () );

#endif
