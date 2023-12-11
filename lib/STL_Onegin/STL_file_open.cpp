#include "STL_file_open.h"

static int CountNumberOfLines (char* text, size_t size);

static void SplitIntoLines (struct File* file);

char* STL_Fread (struct File* file)
{
    assert (file);

    struct stat buff = { 0 };

    file->fp = fopen (file->name, "rb");
    assert (file->fp);

    fstat (fileno (file->fp), &buff);

    file->size = buff.st_size;

    file->text = (char*) calloc (file->size + 1, sizeof (char));
    assert (file->text);

    file->size = fread (file->text, sizeof (char), file->size, file->fp);


    // git undo commit
    // git diff <>
    /**
     * Сhecking for the correctness of entering the last line
     * If the last line does not end with \n,
     * add \n to the end of the buffer and
     * increases the file size by 1
     *
     * Why?
     * Вставим '\n' в конец файла, чтобы не вставлять лишние проверки
     * на каждую итерацию цикла обработки строк.
     * (см. Split....)
     */

     // Split
    if (*(file->text + file->size - 1) != '\n')
    {
        *(file->text + file->size) = '\n';
        (file->size)++;
    }

    fclose (file->fp);
    file->fp = nullptr;

    return file->text;
}

String* STL_SplitFileIntoLines (File* file, const char* name)
{
    assert (file);

    file->name = name;

    file->text = STL_Fread (file);

    file->nLines = CountNumberOfLines (file->text, file->size);

    file->strings = (String*) calloc (file->nLines + 1, sizeof (String));
    assert (file->strings);

    SplitIntoLines (file);

    return file->strings;
}

static int CountNumberOfLines (char* text, size_t size)
{
    size_t numberOfLines = 0;

    char* ptr = text;
    while (ptr != text + size - 1)
    {
        ptr = strchr (ptr + 1, '\n');
        numberOfLines++;
    }

    return numberOfLines;
}

static void SplitIntoLines (File* file)
{
    assert (file);

    size_t line = 0;
    size_t pos = 0;

    (file->strings + line)->str = (file->text + pos);

    line++;
    pos++;

    char* ptr = file->text;

    for (; line < file->nLines + 1; line++)
    {
        ptr = strchr (ptr + 1, '\n');
        pos = ptr - file->text;

        (file->strings + line)    ->str = (file->text + pos + 1);
        (file->strings + line - 1)->len = (file->strings + line)->str
                                          - (file->strings + line - 1)->str - 1;

        pos++;

    }

    (file->strings + line - 1)->len = (file->strings + line)->str
                                      - (file->strings + line - 1)->str - 1;
}

int STL_Fclose (struct File* file)
{
    free (file->text);
    free (file->strings);

    file->name    = 0;
    file->fp      = nullptr;
    file->text    = nullptr;
    file->size    = 0;
    file->nLines  = 0;
    file->strings = nullptr;

    return 0;
}
