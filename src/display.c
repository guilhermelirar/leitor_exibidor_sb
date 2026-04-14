#include "display.h"
#include "classfile.h"
#include <stdio.h>

void print_class_constant_pool(const ClassFile *cf, FILE *file) {
  // assume: cf não nulo, file não nulo
  u2 count = cf->constant_pool_count;
  cp_info* entry = NULL;

  fprintf(file, "Constant Pool:\n");
  for (int i = 1; i < count; i++) {
    entry = &cf->constant_pool[i];

    switch (entry->tag) {
      case CONSTANT_Class: {
        u2 name_index = entry->info.class_info.name_index;
        cp_info* name_entry = &cf->constant_pool[name_index];
        fprintf(file, " #%d (Class) %s\n", i, name_entry->info.utf8_info.bytes);
        break;
      }
      case CONSTANT_Fieldref: 
        break;

      case CONSTANT_Methodref:
        break;
      
      case CONSTANT_InterfaceMethodref:
        break;
      
      case CONSTANT_NameAndType:
        break;

      case CONSTANT_Utf8:
        break; 
     
      case CONSTANT_String:
        break;

      case CONSTANT_Integer:
        break;

      case CONSTANT_Float:
        break;

      case CONSTANT_Long:
        break;

      case CONSTANT_Double: 
        break;

      default:
        fprintf(stderr, "Error: constant tag \"%d\" not recognized\n", entry->tag);
        return;
    }
  } 
}

void printclass(const ClassFile *cf, FILE *file) {
  fprintf(file, "Magic: 0x%X\n", cf->magic);
  fprintf(file, "Version: %d.%d\n", cf->major_version, cf->minor_version);
  fprintf(file, "Constant Pool Count: %d\n", cf->constant_pool_count);
  print_class_constant_pool(cf, file);
}

