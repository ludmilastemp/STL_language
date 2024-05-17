#include "STL_label.h"

#define $printf(...)

static int CreateLabel (Stack_Label* labels, const size_t nLabel, const char* name, const int byte);

static size_t FindLabel   (Stack_Label* labels, const char* string);

static size_t StrlenUpToSpace (const char* const string);

int SetLabel (const char* string, const size_t ip, Stack_Label* labels)
{
    assert (string);
    assert (labels);

$printf ("\n\nI IN SetLabel\n");
$printf ("\tip = %lu\n", ip);

    size_t nLabel = FindLabel (labels, string);

    CreateLabel (labels, nLabel, string, (int)ip);

$printf ("\n\nI END SetLabel\n");

    return 0;
}

int SetLabelArg (const char* string, char* str, size_t* ip, Stack_Label* labels, Stack_Fixup* fixups)
{
    assert (string);
    assert (str);
    assert (ip);
    assert (labels);
    assert (fixups);

$printf ("\n\nI IN SetLabelArg\n");
$printf ("\tip = %lu\n", *ip);

    size_t nLabel = FindLabel (labels, string);

    if (nLabel < labels->size && labels->data[nLabel].address != Label::POISON)
    {
        // *(SPU_DATA_TYPE*)(str + *ip) = labels->data[nLabel].address;
        for (size_t i = 0; i < sizeof (SPU_DATA_TYPE); i++)
        {
            str[*ip + i] = ((const char*)(&labels->data[nLabel].address))[i];
        }

        *ip += sizeof (SPU_DATA_TYPE);

$printf ("\n\nI END111 SetLabelArg\n");

        return 0;
    }

    Fixup newFixup = { *ip, nLabel };
    StackPush (fixups, newFixup);

    if (nLabel == labels->size)
    {
        CreateLabel (labels, nLabel, string, Label::POISON);
    }

    *ip += sizeof (SPU_DATA_TYPE);

$printf ("\n\nI END222 SetLabelArg\n");

    return 0;
}

int DoFixups (char* str, const Stack_Label* labels, const Stack_Fixup* fixups)
{
    assert (labels);
    assert (fixups);

    for (size_t i = 0; i < fixups->size; i++)
    {
        // *(SPU_DATA_TYPE*)(str + fixups->data[i].address) =
        //     labels->data[fixups->data[i].labelIndex].address;
        for (size_t j = 0; j < sizeof (SPU_DATA_TYPE); j++)
        {
            str[fixups->data[i].address + j] = 
                ((const char*)(&labels->data[fixups->data[i].labelIndex].address))[j];
        }
    }

    return 0;
}

int LabelsDump (const Stack_Label* labels)
{
    assert (labels);

    for (size_t i = 0; i < labels->size; ++i)
    {
        $printf ("labels[%lu] \n", i);
        $printf ("\t name = <");
        for (size_t j = 0; j < labels->data[i].lenName; ++j)
        {
            $printf ("%c", (labels->data[i].name)[j]);
        }
        $printf (">\n");
        $printf ("\t lenN = %lu \n", labels->data[i].lenName);
        $printf ("\t byte = %d \n", labels->data[i].address);
    }

    return 0;
}

int FixupsDump (const Stack_Fixup* fixups)
{
    assert (fixups);

    for (size_t i = 0; i < fixups->size; i++)
    {
        $printf ("fixup = %lu\n", i);
        $printf ("\t index = %lu\n", fixups->data[i].labelIndex);
        $printf ("\t byte  = %lu\n", fixups->data[i].address);
    }

    return 0;
}

static int CreateLabel (Stack_Label* labels, const size_t nLabel, const char* name, int byte = -1)
{
    assert (labels);
    assert (name);

$printf ("\n\nI IN CreateLabel\n");

    Label newLabel = { name, StrlenUpToSpace (name), byte };

/*
int len = StrlenUpToSpace (name);
$printf ("\n\tstrlen = %d\n\tname = ", len);
for (int i = 0; i < len; i++)
$printf ("%c", name[i]);
$printf ("\n");*/

    if (nLabel == labels->size)
    {
        $printf ("\t111\n");
        StackPush (labels, newLabel);
    }
    else
    {
        $printf ("\t222\n");
        labels->data[nLabel].address = byte;
    }

$printf ("\n\nI END CreateLabel\n");


    return 0;
}

static size_t FindLabel (Stack_Label* labels, const char* string)
{
    assert (labels);
    assert (string);

    size_t i = 0;

$printf ("\n\nI IN FindLabel\n");

    for (; i < labels->size; ++i)
    {
        if (strncmp (string, labels->data[i].name, labels->data[i].lenName) == 0)
        {
            break;
        }
    }

/*
int len = StrlenUpToSpace (string);
$printf ("\n\tstrlen = %d\n\tname = ", len);
for (int j = 0; j < len; j++)
$printf ("%c", string[j]);
$printf ("\n");

$printf ("\ti    = %d\n", i);
$printf ("\tsize = %d\n", (int)labels->size);
*/
//$printf ("labels->data[i].lenName = %d\n", labels->data[i].lenName);
//for (int j = 0; j < labels->data[i].lenName; ++j)
//    $printf ("labels->data[i].name = %c\n", labels->data[i].name[j]);

$printf ("\n\nI END FindLabel\n");

    return i;
}

static size_t StrlenUpToSpace (const char* const string)
{
    assert (string);

    size_t i = 0;
    while (string[i] != ' ' &&
           string[i] != '\r' &&
           string[i] != '\t' &&
           string[i] != '\n' &&
           string[i] != '\0') i++;

    return i;
}

#undef $printf

