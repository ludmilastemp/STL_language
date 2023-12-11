
#include "running.h"

int main()
{
#ifdef WRITEBINTXT
    FILE *fp = fopen("running.txt", "r");
    assert(fp);

    FILE *fpBinTxt = fopen("running_bintxt.txt", "w");
    assert(fpBinTxt);

    if(!TxtInBinTxt(fp, fpBinTxt)) printf("OK!");

    return 0;
#endif

#ifdef WRITEBIN
    FILE *fp = fopen("running.txt", "r");
    assert(fp);

    FILE *fpBin = fopen("running_bin.txt", "wb");
    assert(fpBin);

    if(!TxtInBin(fp, fpBin)) printf("OK!");

    return 0;
#endif

#ifdef PRINTBIN
    FILE *fpBin = fopen("running_bin.txt", "rb");
    assert(fpBin);

    BinInConsole(fpBin);

    return 0;
#endif
}
