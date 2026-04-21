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
void printclass(const ClassFile* cf, FILE* file);

#endif
