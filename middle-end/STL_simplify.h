#ifndef STL_simplify_
#define STL_simplify_

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <assert.h>

#include "../lib/STL_BinTree/STL_bintree_struct.h"
#include "STL_wolfram.h"

NodeBinTree*
STL_SimplifyEval (NodeBinTree* node, FILE* fp);

int
STL_FoldConst (NodeBinTree* node, FILE* fp);

NodeBinTree*
STL_DeleteNeutralNode (NodeBinTree* node, FILE* fp);

#endif /* STL_simplify_ */
