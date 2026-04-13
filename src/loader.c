#include "loader.h"
#include "classfile.h"
#include <stdio.h>
#include <stdlib.h>
  
u2 read_u2(FILE *file) {
    int high = fgetc(file);
    int low = fgetc(file);

    if (high == EOF || low == EOF) {
        fprintf(stderr, "EOF reached\n");
        return 0;
    }

    return ((u2)high << 8) | (u2)low;
}

u4 read_u4(FILE* file) {
  u2 high = read_u2(file);
  u2 low = read_u2(file);

  return ((u4)high << 16) | (u4)low;
}

ClassFile *load_class(const char *filepath) {
  FILE *file =  fopen(filepath, "rb");
  ClassFile* cf = NULL;

  if (file == NULL) {
    perror("fopen");
    goto cleanup;
  }
  
  cf = (ClassFile*)malloc(sizeof(ClassFile)); // malloc #1
  cf->constant_pool = NULL;
  cf->magic = read_u4(file);
  
  if (cf->magic != (u4)MAGIC) {
    printf("File \"%s\" doesn't contain magic.\n"
        "  Expected: %X\n  Got: %X\n", filepath, MAGIC, cf->magic);
    goto cleanup;
  } 

  cf->minor_version = read_u2(file);
  cf->major_version = read_u2(file);
  cf->constant_pool_count = read_u2(file);
 
  cf->constant_pool = (cp_info*)malloc(sizeof(cp_info) * 
      (cf->constant_pool_count)); // malloc #2

  fclose(file);
  return cf;

cleanup:
  if (file) fclose(file);
  if (cf) free_classfile(cf);
  return NULL;
}

void free_classfile(ClassFile* cf) {
  if (cf == NULL) return;
  if (cf->constant_pool) {
    free(cf->constant_pool);
    cf->constant_pool = NULL;
  }
  free(cf);
}

int read_constant_pool(FILE *file, ClassFile *cf) {
  // assume: cursor logo após constant_pool_count
  cp_info* entry;
  for (int i = 1; i < cf->constant_pool_count; i++) {
    entry = &cf->constant_pool[i];
    entry->tag = (u1)fgetc(file);
    
    switch (entry->tag) {
      case CONSTANT_Class:
        entry->info.class_info.name_index = read_u2(file);  
        break;
      default:
        fprintf(stderr, "Error: constant tag \"0x%X\" not recognized\n", entry->tag);
        return ERR_JAVA_INVALID_TAG;
        break; 
    }
  }

  return SUCCESS;
}
