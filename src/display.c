// TODO printar significado de identificadores de field
#include "display.h"
#include "classfile.h"
#include "bytecode.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

static const access_flag_desc class_flags[] = {
  {ACC_PUBLIC,       "public",       "ACC_PUBLIC"},
  {ACC_FINAL,        "final",        "ACC_FINAL"},
  {ACC_SUPER,        "super",        "ACC_SUPER"},
  {ACC_INTERFACE,    "interface",    "ACC_INTERFACE"},
  {ACC_ABSTRACT,     "abstract",     "ACC_ABSTRACT"},
  {ACC_SYNTHETIC,    "synthetic",    "ACC_SYNTHETIC"},
  {ACC_ANNOTATION,   "annotation",   "ACC_ANNOTATION"},
  {ACC_ENUM,         "enum",         "ACC_ENUM"},
};

static const access_flag_desc method_flags[] = {
  {ACC_PUBLIC,        "public",        "ACC_PUBLIC"},
  {ACC_PRIVATE,       "private",       "ACC_PRIVATE"},
  {ACC_PROTECTED,     "protected",     "ACC_PROTECTED"},
  {ACC_STATIC,        "static",        "ACC_STATIC"},
  {ACC_FINAL,         "final",         "ACC_FINAL"},
  {ACC_SYNCHRONIZED,  "synchronized",  "ACC_SYNCHRONIZED"},
  {ACC_ABSTRACT,      "abstract",      "ACC_ABSTRACT"},
  {ACC_SYNTHETIC,     "synthetic",     "ACC_SYNTHETIC"},
};

static const access_flag_desc field_flags[] = {
  {ACC_PUBLIC,      "public",      "ACC_PUBLIC"},
  {ACC_PRIVATE,     "private",     "ACC_PRIVATE"},
  {ACC_PROTECTED,   "protected",   "ACC_PROTECTED"},
  {ACC_STATIC,      "static",      "ACC_STATIC"},
  {ACC_FINAL,       "final",       "ACC_FINAL"},
  {ACC_VOLATILE,    "volatile",    "ACC_VOLATILE"},
  {ACC_TRANSIENT,   "transient",   "ACC_TRANSIENT"},
  {ACC_SYNTHETIC,   "synthetic",   "ACC_SYNTHETIC"},
  {ACC_ENUM,        "enum",        "ACC_ENUM"},
};


void print_access_flags(u2 bits,
                        access_context_t ctx,
                        access_format_t fmt,
                        FILE* file)
{
  const access_flag_desc* table = NULL;
  size_t count = 0;

  switch (ctx) {
    case ACCESS_CLASS:
      table = class_flags;
      count = sizeof(class_flags) / sizeof(class_flags[0]);
      break;
    case ACCESS_METHOD:
      table = method_flags;
      count = sizeof(method_flags) / sizeof(method_flags[0]);
      break;
    case ACCESS_FIELD:
      table = field_flags;
      count = sizeof(field_flags) / sizeof(field_flags[0]);
      break;
    default:
      return;
  }

  int first = 1;

  const char* sep = (fmt == ACCESS_FMT_JAVA) ? " " : ", ";

  for (size_t i = 0; i < count; i++) {
    if (bits & table[i].mask) {
      if (!first) fputs(sep, file);
      first = 0;
      fputs(
          fmt == ACCESS_FMT_DEBUG ? 
          table[i].debug_kw : table[i].java_kw, 
          file);
    }
  }
}


void print_class_constant_pool(const ClassFile *cf, FILE *file) {
  // assume: cf não nulo, file não nulo
  u2 count = cf->constant_pool_count;
  cp_info* entry = NULL;

  fprintf(file, "Constant Pool (%d):\n", count);
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

        fprintf(file, "Long               %" PRId64, long_hl);
        i++;
        break;
      }

      case CONSTANT_Double: {
        double d;
        u8 bits = ((u8)entry->info.double_info.h_bytes << 32) | 
          ((u8) entry->info.double_info.l_bytes);
        memcpy(&d, &bits, sizeof(double));
        fprintf(file, "Double            %f", d);
        i++;
        break;
      }

      default:
        fprintf(stderr, "Error: constant tag \"%d\" not recognized\n", entry->tag);
        return;
    }
    putc('\n', file);
  } 
}

void print_interfaces(const ClassFile* cf, FILE* file) {
  fprintf(file, "Interfaces: \n");
  for (int i = 0; i < cf->interfaces_count; i++) {
    u2 class_idx = cf->interfaces[i];
    fprintf(file, "  #%20d: (#%d) %s\n", i, class_idx,
        cp_class_name(cf, class_idx));
  }
}

void print_code(const Code_attribute* code, FILE* out, int indent) {
  print_indent(indent, out);
  fprintf(out, "max_stack: %d\n", code->max_stack);
  
  print_indent(indent, out);
  fprintf(out, "max_locals: %d\n", code->max_locals);

  // TODO operandos
  for (u4 i = 0; i < code->code_length; i++) {
    // fprintf("", const char *restrict  _Nonnull format, ...)
  }
  return;
} 

void print_attributes(const ClassFile *cf, u2 count, 
    attribute_info *attributes, FILE *file, int indent) {
  for (int i = 0; i < count; i++) {
    const char *name = cp_get_utf8(cf, attributes[i].attribute_name_index);
        
    // identação
    print_indent(indent, file);
    fprintf(file, "[%d] %s: \n", i, name);
   
    // identificando atributo
    if (strcmp(name, "ConstantValue") == 0) {
      print_indent(indent+4, file);
      fprintf(file, "Constant Value index: #%d\n", attributes[i].info.constantvalue_index);
    } 
    else if (strcmp(name, "Code") == 0) {
      print_indent(indent+4, file);
      fprintf(file, "Bytecode length: %u\n", attributes[i].attribute_length);
      print_code(attributes[i].info.code_attribute, file, indent+6);    
    }
  }
}

void print_class_member(const ClassFile* cf,
  u2 index, u2 name_index, 
  u2 descriptor_index, u2 attributes_count,
  attribute_info* attributes,
  access_context_t access_ctx, u2 access_flags,
  FILE* file) {
  
  fprintf(file, "  [%d] Name: %s\n", index, cp_get_utf8(cf, name_index));
  fprintf(file, "      Descriptor: %s\n", cp_get_utf8(cf, descriptor_index));
  

  fputs("      Access Flags: ", file); 
  print_access_flags(access_flags, access_ctx, ACCESS_FMT_DEBUG, file);
  putc('\n', file);
  
  // Exibe a assinatura em caso de ser método
  if (access_ctx == ACCESS_METHOD) {
    printf("      Definition: ");
    print_method_definition(cf, &cf->methods[index], file);
    putc('\n', file);
  }

  if (attributes_count > 0) {
    fprintf(file, "      Attributes:\n");
    print_attributes(cf, attributes_count, attributes, file, 8);
  }
  
  fprintf(file, "\n");
}

void print_fields(const ClassFile *cf, FILE *file) {
  fprintf(file, "Fields (%d):\n", cf->fields_count);
  for (int i = 0; i < cf->fields_count; i++) {
    field_info *f = &cf->fields[i];
    print_class_member(cf, i, f->name_index, 
        f->descriptor_index, 
        f->attributes_count, 
        f->attributes, 
        ACCESS_FIELD, 
        f->access_flags, file);  
  }
}

void print_methods(const ClassFile *cf, FILE *file) {
  fprintf(file, "Methods (%d):\n", cf->methods_count);
  for (int i = 0; i < cf->methods_count; i++) {
    method_info *m = &cf->methods[i];
    print_class_member(cf, i, m->name_index, 
        m->descriptor_index, 
        m->attributes_count, 
        m->attributes, 
        ACCESS_METHOD, 
        m->access_flags, file);  
  }
} 


void printclass(const ClassFile *cf, FILE *file) {
  fprintf(file, "Magic: 0x%X\n", cf->magic);
  fprintf(file, "Version: %d.%d\n", 
      cf->major_version, cf->minor_version);
  
  putc('\n', file);
  print_class_constant_pool(cf, file);
 
  putc('\n', file);
  fputs("Access Flags: ", file); 
  print_access_flags(cf->access_flags, ACCESS_CLASS, 
      ACCESS_FMT_DEBUG, file);

  putc('\n', file);
  // this_class e super class
  fprintf(file, "This class: %s\n", cp_class_name(cf, cf->this_class));
  fprintf(file, "Super class: %s\n", cp_class_name(cf, cf->super_class));

  // interfaces
  fprintf(file, "Interfaces count: %d\n", cf->interfaces_count);
  if (cf->interfaces_count) print_interfaces(cf, file);

  putc('\n', file);
  print_fields(cf, file);

  putc('\n', file);
  print_methods(cf, file);
}


void print_method_definition(const ClassFile* cf, method_info* m, FILE* out) {
  print_access_flags(m->access_flags, 
        ACCESS_METHOD, ACCESS_FMT_JAVA, out);
  // TODO implementar
  fprintf(out, 
      " <not_implemented>%s(<not implemented>)", 
      cp_get_utf8(cf, m->name_index));
}
