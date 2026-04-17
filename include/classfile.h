#ifndef CLASSFILE
#define CLASSFILE

#include "stdint.h"
#include <inttypes.h>

#define MAGIC 0xCAFEBABE

// constant pool info
#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String 8
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_NameAndType 12
#define CONSTANT_Utf8 1
#define CONSTANT_MethodHandle 15
#define CONSTANT_MethodType 16
#define CONSTANT_InvokeDynamic 18

// access_flags
#define N_OF_ALLOWED_FLAGS 8
#define ACC_PUBLIC 0x0001
#define ACC_FINAL 0x0010
#define ACC_SUPER 0x0020
#define ACC_INTERFACE 0x0200
#define ACC_ABSTRACT 0x0400
#define ACC_SYNTHETIC 0x1000
#define ACC_ANNOTATION 0x2000
#define ACC_ENUM 0x4000

typedef uint8_t  u1;  
typedef uint16_t u2;
typedef uint32_t u4;
typedef uint64_t u8;

typedef struct {
  u2 name_index; 
} CONSTANT_Class_info;

typedef struct {
  u2 class_index;
  u2 name_and_type_index;
} CONSTANT_Fieldref_info;

typedef CONSTANT_Fieldref_info CONSTANT_Methodref_info;
typedef CONSTANT_Fieldref_info CONSTANT_InterfaceMethodref_info;

typedef struct {
  u2 name_index;
  u2 descriptor_index;
} CONSTANT_NameAndType_info;

typedef struct {
  u2 length;
  u1 *bytes;
} CONSTANT_Utf8_info;

typedef struct {
  u2 string_index;
} CONSTANT_String_info;

typedef struct {
  u4 bytes;
} CONSTANT_Integer_info;

typedef CONSTANT_Integer_info CONSTANT_Float_info;

typedef struct {
  u4 h_bytes;
  u4 l_bytes;
} CONSTANT_Long_info;

typedef CONSTANT_Long_info CONSTANT_Double_info;

typedef struct {
  u1 tag;
  union {
    CONSTANT_Class_info class_info;
    CONSTANT_Fieldref_info fieldref_info;
    CONSTANT_Methodref_info methodref_info;
    CONSTANT_InterfaceMethodref_info interface_methodref_info;
    CONSTANT_Utf8_info utf8_info;
    CONSTANT_String_info string_info;
    CONSTANT_NameAndType_info name_and_type_info;
    CONSTANT_Integer_info int_info;
    CONSTANT_Float_info float_info;
    CONSTANT_Double_info double_info;
    CONSTANT_Long_info long_info;
  } info;
} cp_info;

typedef struct {
  u4 magic;
  u2 minor_version;
  u2 major_version;
  u2 constant_pool_count;
  cp_info *constant_pool;
  u2 access_flags;
  u2 this_class;
  u2 super_class;
  u2 interfaces_count;
  u2 *interfaces;
  u2 fields_count;
} ClassFile;

// Obter strings
const char* cp_class_name(const ClassFile *cf, u2 class_index);
const char* cp_nameandtype_name(const ClassFile *cf, u2 nt_index);
const char* cp_get_utf8(const ClassFile *cf, u2 utf8_index);

#endif
