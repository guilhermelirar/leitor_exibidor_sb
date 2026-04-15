#include "display.h"
#include "classfile.h"
#include <stdio.h>

void print_class_constant_pool(const ClassFile *cf, FILE *file) {
  // assume: cf não nulo, file não nulo
  u2 count = cf->constant_pool_count;
  cp_info* entry = NULL;

  fprintf(file, "Constant Pool:\n");
  for (int i = 1; i < count; i++) {
    fprintf(file, " #%d = ", i);
    entry = &cf->constant_pool[i];

    switch (entry->tag) {
      case CONSTANT_Class: {
        u2 name_index = entry->info.class_info.name_index;
        fprintf(file, "Class  (#%d) %s", 
            name_index, cp_class_name(cf, i));
        break;
      }

      case CONSTANT_Fieldref: {
        u2 class_index = entry->info.fieldref_info.class_index;
        u2 nt_index    = entry->info.fieldref_info.name_and_type_index;

        fprintf(file,
          "Fieldref %s.%s",
          cp_class_name(cf, class_index),
          cp_nameandtype_name(cf, nt_index)
        );
        break;
      }
      
      case CONSTANT_Methodref:
        break;
      
      case CONSTANT_InterfaceMethodref:
        break;
      
      case CONSTANT_NameAndType:  {
        u2 nt_index = entry->info.name_and_type_info.name_index;
        u2 desc_index = entry->info.name_and_type_info.descriptor_index;
        fprintf(file, "NameAndType  #%d:%d %s:%s", 
            nt_index, desc_index, 
            cp_nameandtype_name(cf, i), cp_get_utf8(cf, desc_index));
        break;
      }


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
    putc('\n', file);
  } 
}

void printclass(const ClassFile *cf, FILE *file) {
  fprintf(file, "Magic: 0x%X\n", cf->magic);
  fprintf(file, "Version: %d.%d\n", cf->major_version, cf->minor_version);
  fprintf(file, "Constant Pool Count: %d\n", cf->constant_pool_count);
  print_class_constant_pool(cf, file);
}

