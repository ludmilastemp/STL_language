#ifndef __RUNNING__
#define __RUNNING__

#include <stdio.h>
#include <assert.h>

const int MAX_STRING_LENGTH = 250;

int TxtInBinTxt(FILE* fp, FILE* fpBinTxt);

int TxtInBin(FILE* fp, FILE* fpBin);

void BinInConsole(FILE* fp);

#endif
