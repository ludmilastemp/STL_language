#include "running.h"

static char DecToHex(int x);

int TxtInBin(FILE* fp, FILE* fpBin)
{
    assert (fp);
    assert (fpBin);

    int symbol          = 0;
    int symbolTemp      = 0;
    int symbolSameCount = 1;

    //symbol = getc(fp);

    while (true)
    {
        symbol = getc(fp);

        printf ("HI\n");
        //fscanf  (fp, "%d", symbol);
        printf ("BI\n");
        if      (symbol == EOF) return 0;

        printf ("%d", symbol);
        fprintf (fpBin, "%d", symbol);

        /*

        symbolTemp = getc(fp);

        if      (symbolTemp == EOF) return 0;

        else if (symbolTemp == symbol)
        {
            ++symbolSameCount;
        }
        else if (symbolSameCount == 1)
        {
            fprintf(fpBin, "%c", symbol);

            symbol = symbolTemp;
        }
        else
        {
            symbol = symbol | 0x80;

            fprintf(fpBin, "%c", symbol);
            fprintf(fpBin, "%d", symbolSameCount);

            symbol = symbolTemp;
            symbolSameCount = 1;
        }  */
    }

    return 0;
}

int TxtInBinTxt(FILE* fp, FILE* fpBinTxt)
{
    assert (fp);
    assert (fpBinTxt);

    int symbol          = 0;
    int symbolTemp      = 0;
    int symbolSameCount = 1;

    symbol = getc(fp);

    while (true)
    {
        symbolTemp = getc(fp);

        if      (symbolTemp == '\n') fprintf(fpBinTxt, "\n");

        if      (symbolTemp == EOF) return 0;
        else if (symbolTemp == symbol)
        {
            ++symbolSameCount;
        }
        else if (symbolSameCount == 1)
        {
            fprintf(fpBinTxt, "%c",  DecToHex(symbol / 16));
            fprintf(fpBinTxt, "%c ", DecToHex(symbol % 16));

            symbol = symbolTemp;
        }
        else
        {
            symbol += 128;

            fprintf(fpBinTxt, "%c",  DecToHex(symbol / 16));
            fprintf(fpBinTxt, "%c ", DecToHex(symbol % 16));

            fprintf(fpBinTxt, "%c",  DecToHex(symbolSameCount / 16));
            fprintf(fpBinTxt, "%c ", DecToHex(symbolSameCount % 16));

            symbol = symbolTemp;
            symbolSameCount = 1;
        }
    }

    return 0;
}

void BinInConsole(FILE* fp)
{
    assert (fp);

    int symbol   = 0;
    int quantity = 0;

    while (true)
    {
        symbol = getc(fp);

        if (symbol == EOF) break;

        if (symbol >> 7 == 1)
        {
            quantity = getc(fp);

            for(int i = 0; i < quantity; ++i)
            {
                printf("%c", symbol - 128);
            }
        }
        else
        {
            printf("%c", symbol);
        }
    }
}

char DecToHex(int x)
{
    char mass[] = {"0123456789ABCDEF"};

    assert(0 <= x && x < 16);

    return mass[x];
}
