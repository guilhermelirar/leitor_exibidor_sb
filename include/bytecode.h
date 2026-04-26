#ifndef BYTECODE_H
#define BYTECODE_H

#include "classfile.h"

void parse_code_attribute(
  const attribute_info *attr,
  Code_attribute* code_attr
);

void free_code_attribute(Code_attribute *code);

#endif
