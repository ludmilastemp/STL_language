#ifndef STL_graphviz_
#define STL_graphviz_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../../lib/STL_BinTree/STL_bintree_struct.h"

enum
{
    after_front_end = 1,
    after_back_end  = 2,
};

void
STL_GraphvizBinTree (BinTree* binTree, const int file);

#endif /* STL_graphviz_ */
