#ifndef DISPLAY
#define DISPLAY

#include "classfile.h"
#include <stdio.h>

void print_class_constant_pool(const ClassFile *cf, FILE *file);
void print_access_flags(u2 bits, FILE* file);
void print_interfaces(const ClassFile* cf, FILE* file);
void printclass(const ClassFile* cf, FILE* file);

#endif
