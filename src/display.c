#include "display.h"
#include "classfile.h"
#include <stdio.h>

void printclass(const ClassFile *cf, FILE *file) {
  fprintf(file, "Magic: 0x%X\n", cf->magic);
  fprintf(file, "Version: %d.%d\n", cf->major_version, cf->minor_version);
  fprintf(file, "Constant Pool Count: %d\n", cf->constant_pool_count);

}

