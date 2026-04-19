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
        fprintf(file, "Class              (#%d) %s", 
            name_index, cp_class_name(cf, i));
        break;
      }

      case CONSTANT_Fieldref: {
        u2 class_index = entry->info.fieldref_info.class_index;
        u2 nt_index    = entry->info.fieldref_info.name_and_type_index;

        fprintf(file, "Fieldref           %s.%s",
          cp_class_name(cf, class_index),
          cp_nameandtype_name(cf, nt_index)
        );
        break;
      }
      
      case CONSTANT_Methodref: {
        u2 class_index = entry->info.methodref_info.class_index;
        u2 nt_index    = entry->info.methodref_info.name_and_type_index;

        fprintf(file, "Methodref          %s.%s",
          cp_class_name(cf, class_index),
          cp_nameandtype_name(cf, nt_index)
        );
        break;
      }
      
      case CONSTANT_InterfaceMethodref:{
        u2 class_index = entry->info.interface_methodref_info.class_index;
        u2 nt_index    = entry->info.interface_methodref_info.name_and_type_index;

        fprintf(file, "InterfaceMethodref %s.%s",
          cp_class_name(cf, class_index),
          cp_nameandtype_name(cf, nt_index)
        );
        break;
      }
      
      case CONSTANT_NameAndType:  {
        u2 nt_index = entry->info.name_and_type_info.name_index;
        u2 desc_index = entry->info.name_and_type_info.descriptor_index;
        fprintf(file, "NameAndType        #%d:%d %s:%s", 
            nt_index, desc_index, 
            cp_nameandtype_name(cf, i), 
            cp_get_utf8(cf, desc_index));
        break;
      }

      case CONSTANT_Utf8: {
        fprintf(file, "Utf8               %s", 
            cp_get_utf8(cf, i));
        break;
      }
     
      case CONSTANT_String: {
        u2 utf8_idx = entry->info.string_info.string_index;
        fprintf(file, "String             (#%d) %s", 
            utf8_idx, cp_get_utf8(cf, utf8_idx));
        break;
      }

      case CONSTANT_Integer:
        fprintf(file, "Integer            %d", 
            entry->info.int_info.bytes);
        break;

      case CONSTANT_Float: {
        float f;
        u4 bits = entry->info.float_info.bytes;
        memcpy(&f, &bits, sizeof(float));
        fprintf(file, "Float              %f", f);
        break;
      }
      
      case CONSTANT_Long: {
        u8 long_hl = ((u8)entry->info.long_info.h_bytes << 32) | 
          (u8)entry->info.long_info.l_bytes; 

        fprintf(file, "Long              %" PRId64, long_hl);
        break;
      }

      case CONSTANT_Double: {
        double d;
        u8 bits = ((u8)entry->info.double_info.h_bytes << 32) | 
          ((u8) entry->info.double_info.l_bytes);
        memcpy(&d, &bits, sizeof(double));
        fprintf(file, "Double            %f", d);
        break;
      }

      default:
        fprintf(stderr, "Error: constant tag \"%d\" not recognized\n", entry->tag);
        return;
    }
    putc('\n', file);
  } 
}

void print_access_flags(u2 bits, FILE* file) {
  struct {
    u2 mask;
    const char *name;
  } table[] = {
    {ACC_PUBLIC, "ACC_PUBLIC"},
    {ACC_PRIVATE, "ACC_PRIVATE"},
    {ACC_PROTECTED, "ACC_PROTECTED"},
    {ACC_STATIC, "ACC_STATIC"},
    {ACC_FINAL, "ACC_FINAL"},
    {ACC_SUPER, "ACC_SUPER"},
    {ACC_VOLATILE, "ACC_VOLATILE"},
    {ACC_TRANSIENT, "ACC_TRANSIENT"},
    {ACC_INTERFACE, "ACC_INTERFACE"},
    {ACC_ABSTRACT, "ACC_ABSTRACT"},
    {ACC_SYNTHETIC, "ACC_SYNTHETIC"},
    {ACC_ANNOTATION, "ACC_ANNOTATION"},
    {ACC_ENUM, "ACC_ENUM"},
  };
  
  fprintf(file, "Access Flags: ");
  int first = 1;
  for (size_t i = 0; i < (sizeof(table) / sizeof(table[0])); i++) {
    if (bits & table[i].mask) {
      if (!first) fprintf(file, ", ");
      if (first) first = 0;
      fprintf(file, "%s", table[i].name);    
    }
  }
  putc('\n', file);
}

void print_interfaces(const ClassFile* cf, FILE* file) {
  fprintf(file, "Interfaces: \n");
  for (int i = 0; i < cf->interfaces_count; i++) {
    u2 class_idx = cf->interfaces[i];
    fprintf(file, "  #%20d: (#%d) %s\n", i, class_idx,
        cp_class_name(cf, class_idx));
  }
}

void print_attributes(const ClassFile *cf, u2 count, 
    attribute_info *attributes, FILE *file, int indent) {
  for (int i = 0; i < count; i++) {
    const char *name = cp_get_utf8(cf, attributes[i].attribute_name_index);
        
    // identação
    for(int s=0; s<indent; s++) fprintf(file, " ");
        
    fprintf(file, "[%d] %s: \n", i, name);
   
    // identificando atributo
    if (strcmp(name, "ConstantValue") == 0) {
      u2 cv_index = (attributes[i].info[0] << 8) | attributes[i].info[1];
      for(int s=0; s<indent+4; s++) fprintf(file, " ");
      fprintf(file, "Constant Value index: #%d\n", cv_index);
    } 
    else if (strcmp(name, "Code") == 0) {
      for(int s=0; s<indent+4; s++) fprintf(file, " ");
      fprintf(file, "Bytecode length: %u\n", attributes[i].attribute_length);
      // TODO: disassembly 
    }
  }
}

void print_fields(const ClassFile *cf, FILE *file) {
    fprintf(file, "Fields (%d):\n", cf->fields_count);
    for (int i = 0; i < cf->fields_count; i++) {
        field_info *f = &cf->fields[i];
        fprintf(file, "  [%d] Name: %s\n", i, cp_get_utf8(cf, f->name_index));
        fprintf(file, "      Descriptor: %s\n", cp_get_utf8(cf, f->descriptor_index));
        
        // Reusa sua função de access flags (ajuste-a para não printar o prefixo se quiser)
        fprintf(file, "      Flags: 0x%04X\n", f->access_flags); 
        
        if (f->attributes_count > 0) {
            fprintf(file, "      Attributes:\n");
            print_attributes(cf, f->attributes_count, f->attributes, file, 8);
        }
        fprintf(file, "\n");
    }
}

void printclass(const ClassFile *cf, FILE *file) {
  fprintf(file, "Magic: 0x%X\n", cf->magic);
  fprintf(file, "Version: %d.%d\n", 
      cf->major_version, cf->minor_version);
  fprintf(file, "Constant Pool Count: %d\n",
      cf->constant_pool_count);
  
    putc('\n', file);
  print_class_constant_pool(cf, file);
 
  putc('\n', file);
  print_access_flags(cf->access_flags, file);

  putc('\n', file);
  // this_class e super class
  fprintf(file, "This class: %s\n", cp_class_name(cf, cf->this_class));
  fprintf(file, "Super class: %s\n", cp_class_name(cf, cf->super_class));

  // interfaces
  fprintf(file, "Interfaces count: %d\n", cf->interfaces_count);
  if (cf->interfaces_count) print_interfaces(cf, file);

  putc('\n', file);
  print_fields(cf, file);
}




