#include "bytecode.h"

const bytecode_info bytecode_table[256] = {
#include "opcodes.def"
};
