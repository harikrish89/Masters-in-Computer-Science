/****************************************************************************************/
/* ducksinterface.h                                                                     */
/****************************************************************************************/

#ifndef _ducksinterface
#define _ducksinterface

#include "globals.h"
#include "list.h"
#include "circular.h"

extern status circ_append_duck(list *p_L, int n);
extern list getnextduck(list ducks, list nextduck);

#endif
