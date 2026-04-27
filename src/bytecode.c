#include "bytecode.h"

const opcode_info opcode_table[256] = {
#include "opcodes.def"
};
