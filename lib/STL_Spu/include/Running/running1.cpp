#include <stdio.h>
#include <assert.h>

#define NDEBUG

int PR1(char x)
{
    switch(x)
    {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        case 'A':
            return 10;
        case 'B':
            return 11;
        case 'C':
            return 12;
        case 'D':
            return 13;
        case 'E':
            return 14;
        case 'F':
            return 15;
    }
}

char PR16(int x)
{
//mass
    switch(x)
    {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        case 10:
            return 'A';
        case 11:
            return 'B';
        case 12:
            return 'C';
        case 13:
            return 'D';
        case 14:
            return 'E';
        case 15:
            return 'F';
    }
}

void PR()
{
    FILE *fp = fopen("running_new.txt", "r");
    assert(fp);

    char symbol1 = getc(fp);
    char symbol2 = 0;
    int  kolvo = 0;
    int  symbol = 0;

    while (symbol1 != EOF - '0' && symbol1 != EOF)
    {
        if (symbol1 - '0')
        {
            while(symbol != '\n')
            {
                symbol1 = getc(fp);
                symbol2 = getc(fp);
                symbol = PR1(symbol1) * 16 + PR1(symbol2);

                symbol1 = getc(fp);
                symbol2 = getc(fp);
                kolvo = PR1(symbol1) * 16 + PR1(symbol2);

                for (int i= 0; i < kolvo; ++i)
                {
                     printf("%c", symbol);
                }
            }
        }
        else
        {
            while(symbol != '\n')
            {
                symbol1 = getc(fp);
                symbol2 = getc(fp);
                symbol = PR1(symbol1) * 16 + PR1(symbol2);
                printf("%c", symbol);
            }
        }
        symbol1 = getc(fp);
        symbol = 0;
    }
}

int MyGetc(FILE* fp, int line)
{
    static int counter = 0;
    int result = getc(fp);
    fprintf(stdout, "Counter = %d: %d\n", counter, line);
    counter++;
    return result;
}


#define getc(fp) MyGetc(fp, __LINE__)


int ParseCompressed(FILE* fp)
{

    printf("Compressed line\n");

    int symbol = 0;
    while ((int)symbol != 13)
    {
        int symbol = getc(fp);
        printf("                    %d\n", symbol);
        int n_dups = getc(fp);
        assert(symbol != EOF && n_dups != EOF);
        printf("%c:%d\n", symbol, n_dups);
    }

    return 0;
}

int ParseSimple(FILE* fp)
{
    printf("Simple line\n");

    int symbol = 0;
    while (symbol != '\n')
    {
        int symbol = getc(fp);
        assert(symbol != EOF);
        printf("%c\n", symbol);
    }
}

void PRBIN()
{
    FILE *fp = fopen("running_bin.txt", "r");
    assert(fp);

    //FILE *fph = fopen("running_help.txt", "w");
    //assert(fp);

    int kolvo  = 0;

    for (;;)
    {
        int symbol = getc(fp);
        ++kolvo;
        //assert(symbol != EOF);

        if (symbol == EOF) { printf("\n!%d!\n", kolvo); return;  }

        if (symbol == '1')
        {
            ParseCompressed(fp);
        }
        else if (symbol == '0')
        {
            ParseSimple(fp);
        }
        else
        {
            printf("%d\n", symbol);
            assert(0);
        }
    }

        #if 0
            while (symbol != '\n')
            {
                symbol = getc(fp);
                kolvo = getc(fp);       //ÿ
                //fprintf(fph, "%c", symbol);
                printf("%c!", symbol);

                if (symbol != -1)
                {
                    //printf("Minus counter: %d\n", minus1_counter);
                    assert(0);
                }
                //minus1_counter++;

                for (int i= 0; i < kolvo; ++i)
                {
                     printf("%c", symbol);
                }
            }
        }
        else
        {
            printf("#");
            while (symbol != '\n')
            {
                symbol = getc(fp);
                printf("%c", symbol);
                //fprintf(fph, "%c", symbol);

            }
        }
        symbol = getc(fp);
    }
        #endif
}


int main()
{
    //PR(); return 0;
    PRBIN(); return 0;

    FILE *fp = fopen("running.txt", "r");
    assert(fp);

    FILE *fpNew = fopen("running_new.txt", "w");
    assert(fpNew);

    FILE *fpBin = fopen("running_bin.txt", "wb");
    assert(fpBin);

    char symbol = 0;
    char symbolTemp = 0;
    int  symbolCount = 1;            //fgetc fread
    int str[150] = { 0 };
    int strNew[150] = { 0 };
    int i1 = 0;
    int iNew = 0;
    //int  len = 0;
    //int  lenTemp = 0;


    symbol = getc(fp);
    while(true)
    {
        symbolTemp = getc(fp);
        str[i1++] = symbolTemp;

        if (symbolTemp == EOF) break;
        else if (symbolTemp == symbol)
        {
            ++symbolCount;
        }
        else if (symbolTemp != '\n')
        {

            strNew[iNew++] = symbol;
            strNew[iNew++] = symbolCount;

            symbol = symbolTemp;
            symbolTemp = 0;
            symbolCount = 1;
        }
        else
        {
            strNew[iNew++] = symbol;
            strNew[iNew++] = symbolCount;
            assert(symbolCount <= 255);
            strNew[iNew++] = '\n';
            strNew[iNew++] = 1;

            if (i1 <= iNew)
            {
                fprintf(fpNew, "0 ");
                fprintf(fpBin, "%c", '0');
                for (int i = 0; i < i1; ++i)
                {
                    fprintf(fpNew, "%c", PR16(str[i] / 16));
                    fprintf(fpNew, "%c ", PR16(str[i] % 16));
                    fprintf(fpBin, "%c", str[i]);
                    assert(strNew[i] != -1);
                    assert(strNew[i] != 255);
                    //printf("%d ", str[i]);
                }
                fprintf(fpNew, "\n");
            }
            else
            {
                fprintf(fpNew, "1 ");
                //fwrite fputc   0x1B    123.0      char int 32 0C == 222222222222
                fprintf(fpBin, "%c", '1');
                for (int i = 0; i < iNew; ++i)
                {
                    fprintf(fpNew, "%c", PR16(strNew[i] / 16));
                    fprintf(fpNew, "%c ", PR16(strNew[i] % 16));
                    fprintf(fpBin, "%c", strNew[i]);
                    assert(strNew[i] != -1);
                    assert(strNew[i] != 255);
                    //printf("%d ", strNew[i]);

                }
                fprintf(fpNew, "\n");
            }

            symbol = getc(fp);
            symbolCount = 1;
            i1 = 0;
            iNew = 0;
            str[i1++] = symbol;
        }
    }

    return 0;

}
