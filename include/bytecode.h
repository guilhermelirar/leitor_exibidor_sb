#ifndef BYTECODE_H
#define BYTECODE_H

#include "classfile.h"

typedef struct {
  u2 attribute_name_index;
  u4 attribute_length;
  u2 max_stack;
  u2 max_locals;
  u4 code_length;
  u1* code;
  u2 exception_table_length;
  exception_info* exception_table;
  u2 attributes_count;
  attribute_info* attributes;
} Code_attribute;

void parse_code_attribute(
  const ClassFile *cf,
  const attribute_info *attr,
  Code_attribute* code_attr
);

void free_code_attribute(Code_attribute *code);

#endif
