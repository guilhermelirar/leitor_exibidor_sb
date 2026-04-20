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
void free_methods(ClassFile* cf);
void free_attributes(attribute_info* attributes,
    u2 attributes_count);

int read_constant_pool(FILE* file, ClassFile* classfile);
int read_interfaces(FILE* file, ClassFile* cf);
void read_methods(FILE* file, ClassFile* cf);
void read_attributes(FILE* file, 
    u2 attributes_count, attribute_info* attributes); 
void read_fields(FILE* file, ClassFile* cf);
u1* read_utf8(FILE* file, u2 length);
#endif
