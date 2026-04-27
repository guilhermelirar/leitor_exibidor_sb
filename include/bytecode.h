#ifndef BYTECODE_H
#define BYTECODE_H

#include "classfile.h"

typedef struct {
  const char* name;
  u1 operands;
} bytecode_info;

#endif
