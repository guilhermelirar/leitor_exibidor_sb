#include "bytecode.h"
#include "classfile.h"
#include "reader.h"
#include <string.h>
#include <stdlib.h>

void parse_code_attribute(
  const ClassFile *cf,
  const attribute_info *attr,
  Code_attribute *code_attr
) {
  Reader reader = {
    .buf = attr->info,
    .size = attr->attribute_length,
    .pos = 0
  };

  code_attr->attribute_name_index = attr->attribute_name_index;
  code_attr->attribute_length     = attr->attribute_length;

  code_attr->max_stack  = read_u2(&reader);
  code_attr->max_locals = read_u2(&reader);

  code_attr->code_length = read_u4(&reader);

  code_attr->code = malloc(code_attr->code_length);
  for (u4 j = 0; j < code_attr->code_length; j++) {
    code_attr->code[j] = read_u1(&reader);
  }

  code_attr->exception_table_length = read_u2(&reader);

  code_attr->exception_table =
      calloc(code_attr->exception_table_length, sizeof(exception_info));

  for (u2 e = 0; e < code_attr->exception_table_length; e++) {
    exception_info *ex = &code_attr->exception_table[e];
    ex->start_pc   = read_u2(&reader);
    ex->end_pc     = read_u2(&reader);
    ex->handler_pc = read_u2(&reader);
    ex->catch_type = read_u2(&reader);
  }

  code_attr->attributes_count = read_u2(&reader);

  // TODO parse code attributes
}

void free_code_attribute(Code_attribute *code);
