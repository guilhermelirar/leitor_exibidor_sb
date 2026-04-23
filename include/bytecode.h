#ifndef BYTECODE_H
#define BYTECODE_H

#include "classfile.h"

Code_attribute *parse_code_attribute(
  const ClassFile *cf,
  const attribute_info *attr
);

void free_code_attribute(Code_attribute *code);

#endif
