#ifndef BYTECODE_H
#define BYTECODE_H

#include "classfile.h"
typedef enum { 
    OP_NONE, OP_CP, OP_LOCAL, OP_LITERAL, OP_BRANCH, OP_SPECIAL 
} OpType;

typedef struct {
  const char* name;
  int operands;
  OpType type;
} opcode_info;

extern const opcode_info opcode_table[256];

#endif
