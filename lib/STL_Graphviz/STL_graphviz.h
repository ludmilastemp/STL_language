#ifndef STL_graphviz_
#define STL_graphviz_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <assert.h>

#include "../../lib/STL_Bintree/STL_bintree_struct.h"

void
STL_GraphvizBinTree (BinTree* binTree, const int file);

#endif /* STL_graphviz_ */
