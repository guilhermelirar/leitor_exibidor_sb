#ifndef DISPLAY
#define DISPLAY

#include "classfile.h"
#include <stdio.h>

typedef struct {
  u2 mask;
  const char* name;
} access_flag_desc;

void print_class_constant_pool(const ClassFile *cf, FILE *file);

void print_access_flags(u2 bits, access_context_t ctx, FILE* file);

void print_interfaces(const ClassFile* cf, FILE* file);

void print_fields(const ClassFile* cf, FILE* file);

void print_attributes(const ClassFile *cf, 
    u2 count, attribute_info *attributes, 
    FILE *file, int indent);

void print_methods(const ClassFile* cf, FILE *file);

void print_class_member(const ClassFile* cf,
    u2 index,
    u2 name_index, u2 descriptor_index, u2 attributes_count,
    attribute_info* attributes,
    access_context_t access_ctx, u2 access_flags,
    FILE* file);

void printclass(const ClassFile* cf, FILE* file);

#endif
