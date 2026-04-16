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
        
      case CONSTANT_Fieldref:
        entry->info.fieldref_info.class_index = read_u2(file);
        entry->info.fieldref_info.name_and_type_index = read_u2(file);
        break;

      case CONSTANT_Methodref:
        entry->info.methodref_info.class_index = read_u2(file);
        entry->info.methodref_info.name_and_type_index = read_u2(file);
        break;
      
      case CONSTANT_InterfaceMethodref:
        entry->info.interface_methodref_info.class_index = read_u2(file);
        entry->info.interface_methodref_info.name_and_type_index = read_u2(file);
        break;
      
      case CONSTANT_NameAndType:
        entry->info.name_and_type_info.name_index = read_u2(file);
        entry->info.name_and_type_info.descriptor_index = read_u2(file);
        break;

      case CONSTANT_Utf8:
        entry->info.utf8_info.length = read_u2(file);
        entry->info.utf8_info.bytes = read_utf8(file, 
            entry->info.utf8_info.length);
        if (entry->info.utf8_info.bytes == NULL) return ERR_CONSTANT_POOL_READ;
        break; 
     
      case CONSTANT_String:
        entry->info.string_info.string_index = read_u2(file);
        break;

      case CONSTANT_Integer:
        entry->info.int_info.bytes = read_u4(file);
        break;

      case CONSTANT_Float:
        entry->info.float_info.bytes = read_u4(file);
        break;

      case CONSTANT_Long:
        entry->info.long_info.h_bytes = read_u4(file);
        entry->info.long_info.l_bytes = read_u4(file);
        break;

      case CONSTANT_Double: 
        entry->info.double_info.h_bytes = read_u4(file);
        entry->info.double_info.l_bytes = read_u4(file);
        break;

      default:
        fprintf(stderr, "Error: constant tag \"%d\" not recognized\n", entry->tag);
        return ERR_JAVA_INVALID_TAG;
        break; 
    }
  }

  return SUCCESS;
}

u1* read_utf8(FILE *file, u2 length) {
  u1 *bytes = (u1*) malloc(length + 1); // malloc #3
    
  if (bytes == NULL) {
    return NULL;
  }

  size_t read_count = fread(bytes, 1, length, file);

  if (read_count != length) {
      free(bytes);
      return NULL;
  }

  bytes[length] = '\0';

  return bytes;
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

  if (read_constant_pool(file, cf) != SUCCESS) goto cleanup; // malloc #3

  cf->access_flags = read_u2(file);

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
    cp_info *entry; 

    // Libera memória de strings
    for (int i = 1; i < cf->constant_pool_count; i++) {
      entry = &cf->constant_pool[i];
      if (entry && (entry->tag == CONSTANT_Utf8) 
          && entry->info.utf8_info.bytes) 
        free(entry->info.utf8_info.bytes); // free #3
    }
    
    free(cf->constant_pool); // free #2
    cf->constant_pool = NULL;
  }
  free(cf); // free #1
}
