#include "STL_label.h"

static int CreateLabel (Stack_Label* labels, const int nLabel, const char* name, const int byte);

static int FindLabel   (Stack_Label* labels, const char* string);

static int StrlenUpToSpace (const char* const string);

int SetLabel (const char* string, const int ip, Stack_Label* labels)
{
    assert (string);
    assert (ip >= 0);
    assert (labels);

//printf ("\n\nI IN SetLabel\n");
//printf ("ip = %d\n", ip);

    int nLabel = FindLabel (labels, string);

    CreateLabel (labels, nLabel, string, ip);

    return 0;
}

int SetLabelArg (const char* string, char* str, int* ip, Stack_Label* labels, Stack_Fixup* fixups)
{
    assert (string);
    assert (str);
    assert (ip);
    assert (labels);
    assert (fixups);

//printf ("\n\nI IN SetLabelArg\n");
//printf ("ip = %d\n", *ip);

    int nLabel = FindLabel (labels, string);

    if (nLabel < labels->size && labels->data[nLabel].address != Label::POISON)
    {
        *(SPU_DATA_TYPE*)(str + *ip) = labels->data[nLabel].address;

        *ip += sizeof (SPU_DATA_TYPE);

        return 0;
    }

    Fixup newFixup = { *ip, (int)labels->size };
    StackPush (fixups, newFixup);

    if (nLabel == labels->size)
    {
        CreateLabel (labels, nLabel, string, Label::POISON);
    }

    *ip += sizeof (SPU_DATA_TYPE);

    return 0;
}

int DoFixups (char* str, const Stack_Label* labels, const Stack_Fixup* fixups)
{
    assert (labels);
    assert (fixups);

    for (int i = 0; i < fixups->size; i++)
    {
        *(SPU_DATA_TYPE*)(str + fixups->data[i].address) =
            labels->data[fixups->data[i].labelIndex].address;
    }

    return 0;
}

int LabelsDump (const Stack_Label* labels)
{
    assert (labels);

    for (int i = 0; i < labels->size; ++i)
    {
        printf ("labels[%d] \n", i);
        printf ("\t name = <");
        for (int j = 0; j < labels->data[i].lenName; ++j)
        {
            printf ("%c", (labels->data[i].name)[j]);
        }
        printf (">\n");
        printf ("\t lenN = %d \n", labels->data[i].lenName);
        printf ("\t byte = %d \n", labels->data[i].address);
    }

    return 0;
}

int FixupsDump (const Stack_Fixup* fixups)
{
    assert (fixups);

    for (int i = 0; i < fixups->size; i++)
    {
        printf ("fixup = %d\n", i);
        printf ("\t index = %d\n", fixups->data[i].labelIndex);
        printf ("\t byte  = %d\n", fixups->data[i].address);
    }

    return 0;
}

static int CreateLabel (Stack_Label* labels, const int nLabel, const char* name, int byte = -1)
{
    assert (labels);
    assert (nLabel >= 0);
    assert (name);

    Label newLabel = { name, StrlenUpToSpace (name), byte };

//printf ("\n!!!!!!\tstrlen = %d\n\n", StrlenUpToSpace (name));

    if (nLabel == labels->size)
    {
        StackPush (labels, newLabel);
    }
    else
    {
        labels->data[nLabel].address = byte;
    }

    return 0;
}

static int FindLabel (Stack_Label* labels, const char* string)
{
    assert (labels);
    assert (string);

    int i = 0;

//printf ("I IN FindLabel\n");

    for (; i <= labels->size - 1; ++i)
    {
        if (strncmp (string, labels->data[i].name, labels->data[i].lenName) == 0)
        {
            break;
        }
    }

//printf ("           i = %d\n", (int)labels->size);
//printf ("labels->size = %d\n", (int)labels->size);
//printf ("labels->data[i].lenName = %d\n", labels->data[i].lenName);
//for (int j = 0; j < labels->data[i].lenName; ++j)
//    printf ("labels->data[i].name = %c\n", labels->data[i].name[j]);

    return i;
}

static int StrlenUpToSpace (const char* const string)
{
    assert (string);

    int i = 0;
    while (string[i] != ' ' &&
           string[i] != '\r' &&
           string[i] != '\t' &&
           string[i] != '\n' &&
           string[i] != '\0') i++;

    return i;
}

