#ifndef DISPLAY
#define DISPLAY

#include "classfile.h"
#include <stdio.h>

void print_class_constant_pool(const ClassFile *cf, FILE *file);
void printclass(const ClassFile* cf, FILE* file);

// Obter strings
const char* cp_class_name(const ClassFile *cf, u2 class_index);
const char* cp_nameandtype_name(const ClassFile *cf, u2 nt_index);
const char* cp_get_utf8(const ClassFile *cf, u2 utf8_index);

#endif
