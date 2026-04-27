#ifndef BYTECODE_H
#define BYTECODE_H

#include "classfile.h"

typedef struct {
  const char* name;
  u1 operands;
} opcode_info;

extern const opcode_info opcode_table[256];

#endif
