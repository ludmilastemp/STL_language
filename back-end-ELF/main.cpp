#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <elf.h>

#include "../back-end-ELF/back-end.h"
#include "../lib/STL_Graphviz/STL_graphviz.h"

const int memorySize = 0x1000;

void CompileByteCode (char* executeBuf, File* file);
void CompileELFFile  (char* executeBuf, FILE* fp);

int main (const int argc, const char** argv)
{
    const char* argv1 = 0;
    const char* argv2 = 0;

    if (argc <= 1) argv1 = "examples/tree.ast";
    else argv1 = argv[1];

    if (argc <= 2) argv2 = "examples/elf.exe";
    else argv2 = argv[2];

    File file = { .name = argv1 };

    void* executeBuf = mmap (0, memorySize, PROT_EXEC | PROT_READ | PROT_WRITE , MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
    if (executeBuf == MAP_FAILED)
    {
        printf ("By ERROR!\n");
    }
    
    CompileByteCode ((char*)executeBuf, &file);

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

    int (*Func)() = (int(*)())executeBuf; 
    printf ("%d\n", Func ());

    CompileELFFile ((char*)executeBuf, fp);
    fclose (fp);

    if (munmap (executeBuf, memorySize) != 0)
    {
        printf ("munmap FAIL %s\n", strerror(errno));
    }

    printf ("\n\nОК!!!\n\n");

    return 0;
}

static void MainRetFixup (unsigned char* executeBuf, FILE* fp, size_t BufferOffset);

void CompileELFFile (char* executeBuf1, FILE* fp)
{
    assert (executeBuf1);
    assert (fp);


    unsigned char executeBuf[memorySize] = { 0 };
    memcpy (executeBuf, executeBuf1, memorySize);

    const Elf64_Half nPheaders = 2;
    const Elf64_Half nSheaders = 3;
    const Elf64_Half SheaderStrTabIndex = 0x02;

    const Elf64_Addr SVAddr = 0x0000000000400000;
    const Elf64_Addr SPAddr = SVAddr;
    const Elf64_Xword SAlign = 0x1000;
    const uint32_t strtabSize = 0x0000000000000020;
    const uint32_t BufferOffset = SAlign;

    uint32_t BufferSize = 0; 
    Elf64_Off SheaderOffset = 0;

    BufferSize = memorySize;  
    SheaderOffset = BufferOffset + BufferSize + strtabSize;

    Elf64_Ehdr ELFheader = {};
    memset (&ELFheader, 0, sizeof (Elf64_Ehdr));
    memcpy (&ELFheader.e_ident + 0, &ELFMAG, strlen (ELFMAG));
    ELFheader.e_ident[EI_CLASS]   = ELFCLASS64;
    ELFheader.e_ident[EI_DATA]    = ELFDATA2LSB;
    ELFheader.e_ident[EI_VERSION] = EV_CURRENT;
    ELFheader.e_ident[EI_OSABI]   = ELFOSABI_NONE;
    ELFheader.e_type      = ET_EXEC;                              // (10 - 11) 
    ELFheader.e_machine   = EM_X86_64;                            // (12 - 13)
    ELFheader.e_version   = EV_CURRENT;                           // (14 - 17)
    ELFheader.e_entry     = SVAddr + BufferOffset;                // (18 - 1f) 
    ELFheader.e_phoff     = sizeof (Elf64_Ehdr);                  // (20 - 27)
    ELFheader.e_shoff     = SheaderOffset;                        // (28 - 2f)
    ELFheader.e_flags     = 0;                                    // (30 - 33) 
    ELFheader.e_ehsize    = sizeof (Elf64_Ehdr);                  // (34 - 35)
    ELFheader.e_phentsize = sizeof (Elf64_Phdr);                  // (36 - 37)
    ELFheader.e_phnum     = nPheaders;                            // (38 - 39)
    ELFheader.e_shentsize = sizeof (Elf64_Shdr);                  // (3a - 3b)
    ELFheader.e_shnum     = nSheaders;                            // (3c - 3d)
    ELFheader.e_shstrndx  = SheaderStrTabIndex;                   // (3e - 3f)

    Elf64_Phdr Pheaders[nPheaders] = {}; 
    memset (&Pheaders, 0, sizeof (Elf64_Phdr) * nPheaders);

    Pheaders[0].p_type   = PT_LOAD;                // (40 - 43)
    Pheaders[0].p_flags  = PF_R;                   // (44 - 47)
    Pheaders[0].p_offset = 0;                      // (48 - 4f)
    Pheaders[0].p_vaddr  = SVAddr;                 // (50 - 57)
    Pheaders[0].p_paddr  = SPAddr;                 // (58 - 5f)
    Pheaders[0].p_filesz = 0x00000000000000b0;     // (60 - 67)
    Pheaders[0].p_memsz  = 0x00000000000000b0;     // (68 - 6f)
    Pheaders[0].p_align  = SAlign;                 // (70 - 77)
    
    Pheaders[1].p_type   = PT_LOAD;                // (78 - 7b)
    Pheaders[1].p_flags  = PF_R | PF_X;            // (7c - 7f) 
    Pheaders[1].p_offset = BufferOffset;             // (80 - 87)
    Pheaders[1].p_vaddr  = SVAddr + BufferOffset;    // (88 - 8f)
    Pheaders[1].p_paddr  = SPAddr + BufferOffset;    // (90 - 97)
    Pheaders[1].p_filesz = BufferSize;             // (97 - 9f)
    Pheaders[1].p_memsz  = BufferSize;             // (A0 - A7)
    Pheaders[1].p_align  = SAlign;                 // (A8 - Af)

    Elf64_Shdr Sheaders[nSheaders] = {};
    memset (&Sheaders, 0, sizeof (Elf64_Shdr) * nSheaders);
    Sheaders[0].sh_name      = 0;                   // (1020 - 1023)
    Sheaders[0].sh_type      = SHT_NULL;            // (1024 - 1027) 
    Sheaders[0].sh_flags     = 0;                   // (1028 - 102f)
    Sheaders[0].sh_addr      = 0;                   // (1030 - 1037)
    Sheaders[0].sh_offset    = 0;                   // (1038 - 103f)
    Sheaders[0].sh_size      = 0;                   // (1040 - 1047)
    Sheaders[0].sh_link      = 0;                   // (1048 - 104b)
    Sheaders[0].sh_info      = 0;                   // (104c - 104f)
    Sheaders[0].sh_addralign = 0;                   // (1050 - 1057)
    Sheaders[0].sh_entsize   = 0;                   // (1058 - 105f)

    /// .text
    Sheaders[1].sh_name      = 0x0b;                // (1060 - 1063)
    Sheaders[1].sh_type      = SHT_PROGBITS;        // (1064 - 1067)
    Sheaders[1].sh_flags     = SHF_ALLOC | SHF_EXECINSTR; // (1068 - 106f) 
    Sheaders[1].sh_addr      = SVAddr + BufferOffset; // (1070 - 1077)
    Sheaders[1].sh_offset    = BufferOffset;          // (1078 - 107f)
    Sheaders[1].sh_size      = BufferSize;          // (1080 - 1087)
    Sheaders[1].sh_link      = 0;                   // (1088 - 108b)
    Sheaders[1].sh_info      = 0;                   // (108c - 108f)
    Sheaders[1].sh_addralign = 64;                  // (1090 - 1097)
    Sheaders[1].sh_entsize   = 0;                   // (1098 - 109f)

    /// .shstrtab // dg info
    Sheaders[2].sh_name      = 0x01;                // (10a0 - 10a3)
    Sheaders[2].sh_type      = SHT_STRTAB;          // (10a4 - 10a7) 
    Sheaders[2].sh_flags     = 0;                   // (10a8 - 10af)
    Sheaders[2].sh_addr      = 0;                   // (10b0 - 10b7)
    Sheaders[2].sh_offset    = BufferOffset + BufferSize; // (10b8 - 10bf)
    Sheaders[2].sh_size      = strtabSize;          // (10c0 - 10c7)
    Sheaders[2].sh_link      = 0;                   // (10c8 - 10cb)
    Sheaders[2].sh_info      = 0;                   // (10cc - 10cf)
    Sheaders[2].sh_addralign = 1;                   // (10d0 - 10d7)
    Sheaders[2].sh_entsize   = 0;                   // (10d8 - 10df)

    const char* strtab[] = { "", ".shstrtab", ".text" };

    fwrite (&ELFheader, sizeof (Elf64_Ehdr), 1, fp);

    for (int i = 0; i < nPheaders; i++)
        fwrite (&Pheaders[i], sizeof (Elf64_Phdr), 1, fp);

    fseek (fp, BufferOffset, SEEK_SET);
    fwrite (executeBuf, memorySize, 1, fp);

    MainRetFixup (executeBuf, fp, BufferOffset);

    for (int i = 0; i < nSheaders; i++)
        fwrite (strtab[i], strlen (strtab[i]) + 1, 1, fp);

    fseek (fp, SheaderOffset, SEEK_SET);

    for (int i = 0; i < nSheaders; i++)
        fwrite (&Sheaders[i], sizeof (Elf64_Shdr), 1, fp);

    return;
}

void MainRetFixup (unsigned char* executeBuf, FILE* fp, size_t BufferOffset)
{
    assert (executeBuf);
    assert (fp);

    int mainRetIndex = 0;
    for (mainRetIndex = 0; mainRetIndex < memorySize; mainRetIndex++)
    {
        if (((unsigned char*)executeBuf)[mainRetIndex] == 0x5d && ((unsigned char*)executeBuf)[mainRetIndex + 1] == 0xc3)
        {
            mainRetIndex++;
            break;
        }
    }

    char exit[20] = { 0x48, 0x89, 0xC7, 0x48, 0xC7, 0xC0, 0x3C, 0x00, 0x00, 0x00, 0x0F, 0x05 };

    fseek (fp, BufferOffset + mainRetIndex, SEEK_SET);
    fwrite (exit, sizeof (exit), 1, fp);

    fseek (fp, BufferOffset + memorySize, SEEK_SET);
}

void CompileByteCode (char* executeBuf, File* file)
{
    assert (executeBuf);
    assert (file);


}
