#ifndef LOADER
#define LOADER

#include "classfile.h"
#include "stdio.h"
#include "stdlib.h"
#include "reader.h"

#define SUCCESS 0
#define ERR_JAVA_INVALID_TAG -1 
#define ERR_CONSTANT_POOL_READ -2

// Função principal
ClassFile* load_class(const char* fpath);
ClassFile* read_class(Reader *r);

// Liberação de memória
void free_classfile(ClassFile* classfile);
void free_constant_pool(ClassFile* cf);
void free_fields(ClassFile* cf);
void free_methods(ClassFile* cf);
void free_attributes(attribute_info* attributes,
    u2 attributes_count);

int read_constant_pool(Reader *file, ClassFile *cf);
int read_interfaces(Reader* file, ClassFile* cf);
void read_methods(Reader* file, ClassFile* cf);
void read_attributes(Reader* file, 
    u2 attributes_count, attribute_info* attributes); 
void read_fields(Reader* file, ClassFile* cf);
u1* read_utf8(Reader* file, u2 length);
#endif
