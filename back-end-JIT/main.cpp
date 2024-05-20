#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>

#include "../back-end-ELF/back-end.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

const int memorySize = 0x1000;

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;
    const char* argv2 = 0;
    
    if (argc <= 1) argv1 = "examples/tree.ast";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/jit.bin";
    else argv2 = argv[2];

    void* executeBuf = mmap (0, memorySize, PROT_EXEC | PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (executeBuf == MAP_FAILED)
    {
        printf ("By ERROR!\n");
    }

    if (argc > 1 && strcmp (argv[1], "--precompiled") == 0)
    {
        FILE* fp = fopen (argv2, "rb");
        assert (fp);

        fread (executeBuf, sizeof (char), memorySize, fp);
        fclose (fp);
    } 
    else
    {
        File file = { .name = argv1 };

        Stack_Variable var = { 0 };
        StackCtor (&var);

        BinTree* tree = BinTreeReadPostorderWithoutNil (&file, &var);
        if (tree == nullptr)
        {
            printf ("TREE NULLPTR!!!\n\n");
            return 0;
        }

        STL_GraphvizBinTree (tree, after_back_end);


        BackEndCtx ctx = { .node       = tree->root,
                        .fp         = nullptr,
                        .var        = &var,
                        .nIf        = 1,
                        .nWhile     = 1,
                        .nVarInFunc = 0, 
                        .buffer     = (char*)executeBuf,  
                        .fixupSize  = 0
                            };

        CompileProgram (&ctx);

        StackDtor (&var);
        BinTreeDtor (tree);
        STL_Fclose (&file);

        FILE* fp = fopen (argv2, "w");
        assert (fp);

        fwrite (executeBuf, memorySize, 1, fp);
        fclose (fp);
    }

    int (*Func)() = (int(*)())executeBuf; 
    printf ("%d\n", Func ());

    if (munmap (executeBuf, memorySize) != 0)
    {
        printf ("munmap FAIL %s\n", strerror(errno));
    }

    printf ("\n\nОК!!!\n\n");

    return 0;
}
