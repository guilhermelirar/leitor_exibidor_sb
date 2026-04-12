#ifndef CLASSFILE
#define CLASSFILE

#include "stdint.h"

#define MAGIC 0xCAFEBABE

typedef uint8_t  u1;  
typedef uint16_t u2;
typedef uint32_t u4;

typedef struct {
  u4 magic;
  u2 minor_version;
  u2 major_version;
  u2 constant_pool_count;
} ClassFile;

#endif
