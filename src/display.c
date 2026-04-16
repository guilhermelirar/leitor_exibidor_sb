#include "display.h"
#include "classfile.h"
#include <stdio.h>
#include <string.h>

void print_class_constant_pool(const ClassFile *cf, FILE *file) {
  // assume: cf não nulo, file não nulo
  u2 count = cf->constant_pool_count;
  cp_info* entry = NULL;

  fprintf(file, "Constant Pool:\n");
  for (int i = 1; i < count; i++) {
    fprintf(file, " #%02d = ", i);
    entry = &cf->constant_pool[i];

    switch (entry->tag) {
      case CONSTANT_Class: {
        u2 name_index = entry->info.class_info.name_index;
        fprintf(file, "Class        (#%d) %s", 
            name_index, cp_class_name(cf, i));
        break;
      }

      case CONSTANT_Fieldref: {
        u2 class_index = entry->info.fieldref_info.class_index;
        u2 nt_index    = entry->info.fieldref_info.name_and_type_index;

        fprintf(file, "Fieldref     %s.%s",
          cp_class_name(cf, class_index),
          cp_nameandtype_name(cf, nt_index)
        );
        break;
      }
      
      case CONSTANT_Methodref: {
        u2 class_index = entry->info.methodref_info.class_index;
        u2 nt_index    = entry->info.methodref_info.name_and_type_index;

        fprintf(file, "Fieldref     %s.%s",
          cp_class_name(cf, class_index),
          cp_nameandtype_name(cf, nt_index)
        );
        break;
      }
      
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

      case CONSTANT_Utf8: {
        fprintf(file, "Utf8         %s", cp_get_utf8(cf, i));
        break;
      }
     
      case CONSTANT_String: {
        u2 utf8_idx = entry->info.string_info.string_index;
        fprintf(file, "String       (#%d) %s", utf8_idx, cp_get_utf8(cf, utf8_idx));
        break;
      }

      case CONSTANT_Integer:
        fprintf(file, "Integer      %d", entry->info.int_info.bytes);
        break;

      case CONSTANT_Float: {
        float f;
        u4 bits = entry->info.float_info.bytes;
        memcpy(&f, &bits, sizeof(float));
        fprintf(file, "Float        %f", f);
        break;
      }
      
      case CONSTANT_Long: {
        u8 long_hl = ((u8)entry->info.long_info.h_bytes << 32) | 
          (u8)entry->info.long_info.l_bytes; 

        fprintf(file, "Long         %" PRId64, long_hl);
        break;
      }

      case CONSTANT_Double: {
        double d;
        u8 bits = ((u8)entry->info.double_info.h_bytes << 32) | 
          ((u8) entry->info.double_info.l_bytes);
        memcpy(&d, &bits, sizeof(double));
        fprintf(file, "Double       %f", d);
        break;
      }

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

