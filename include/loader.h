#ifndef LOADER
#define LOADER

#include "classfile.h"
#include "stdio.h"
#include "stdlib.h"

#define SUCCESS 0
#define ERR_JAVA_INVALID_TAG -1 
#define ERR_CONSTANT_POOL_READ -2

// Helpers de leitura
u2 read_u2(FILE *file);
u4 read_u4(FILE *file);

// Função principal
ClassFile* load_class(const char* fpath);

// Liberação de memória
void free_classfile(ClassFile* classfile);
void free_constant_pool(ClassFile* cf);
void free_fields(ClassFile* cf);

int read_constant_pool(FILE* file, ClassFile* classfile);
int read_interfaces(FILE* file, ClassFile* cf);
void read_field_attributes(FILE* file, 
    ClassFile* cf, int field_idx);
void read_fields(FILE* file, ClassFile* cf);
u1* read_utf8(FILE* file, u2 length);
#endif
