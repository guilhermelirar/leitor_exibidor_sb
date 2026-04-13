#ifndef LOADER
#define LOADER

#include "classfile.h"
#include "stdio.h"
#include "stdlib.h"

#define SUCCESS 0
#define ERR_JAVA_INVALID_TAG -1

u2 read_u2(FILE *file);
u4 read_u4(FILE *file);
ClassFile* load_class(const char* fpath);
void free_classfile(ClassFile* classfile);
int read_constant_pool(FILE* file, ClassFile* classfile);

#endif
