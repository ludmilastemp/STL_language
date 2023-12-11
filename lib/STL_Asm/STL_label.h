#ifndef STL_label_
#define STL_label_

#include "../STL_Spu/include/STL_spu_struct_const.h"
#include "STACK_FILE_TYPE_label.h"
#include "../STL_stack/generic.h"
#include "STACK_FILE_TYPE_fixup.h"
#include "../STL_stack/generic.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

int SetLabel    (const char* string, const int ip, Stack_Label* labels);

int SetLabelArg (const char* string, char* str, int* ip, Stack_Label* labels, Stack_Fixup* fixups);

int DoFixups    (char* str, const Stack_Label* labels, const Stack_Fixup* fixups);

int LabelsDump (const Stack_Label* labels);

int FixupsDump (const Stack_Fixup* fixups);

#endif /* STL_label_ */
