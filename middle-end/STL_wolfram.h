#ifndef STL_wolfram_
#define STL_wolfram_

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/STL_BinTree/STL_bintree_struct.h"
#include "STL_simplify.h"

double
Eval (NodeBinTree* node, FILE* fp);

BinTree*
Differentiate (BinTree* binTree, FILE* fp);

NodeBinTree*
PartialDerivative (NodeBinTree* node, int variable, FILE* fp);

BinTree*
Taylor (BinTree* binTree, int accuracy, double coordinate, FILE* fp);

int
IsConstData (NodeBinTree* node);

#endif /* STL_wolfram_ */
