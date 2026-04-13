#include "loader.h"
#include "classfile.h"
#include <stdlib.h>
  
u2 read_u2(FILE *file) {
    int high = fgetc(file);
    int low = fgetc(file);

    if (high == EOF || low == EOF) {
        fprintf(stderr, "EOF reached\n");
        exit(1);
    }

    return ((u2)high << 8) | (u2)low;
}

u4 read_u4(FILE* file) {
  u2 high = read_u2(file);
  u2 low = read_u2(file);

  return ((u4)high << 16) | (u4)low;
}

ClassFile *load_class(const char *filepath) {
  ClassFile* class = (ClassFile*)malloc(sizeof(ClassFile));
  FILE *file =  fopen(filepath, "rb");

  if (file == NULL) {
    perror("fopen");
    free_classfile(class);
    exit(1);
  }

  class->magic = read_u4(file);
  
  if (class->magic != (u4)MAGIC) {
    printf("File \"%s\" doesn't contain magic.\n"
        "  Expected: %X\n  Got: %X\n", filepath, MAGIC, class->magic);
    fclose(file);
    free_classfile(class);
    exit(1);
  } 

  class->minor_version = read_u2(file);
  class->major_version = read_u2(file);
  class->constant_pool_count = read_u2(file);
  
  fclose(file);
  return class;
}

void free_classfile(ClassFile* classfile) {
  // TMP
  free(classfile);
}
