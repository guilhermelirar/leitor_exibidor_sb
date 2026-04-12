#include "stdio.h"
#include "classfile.h"
#include "display.h"
#include <stdlib.h>

u2 read_u2(FILE *file) {
    int high = fgetc(file);
    int low = fgetc(file);

    if (high == EOF || low == EOF) {
        fprintf(stderr, "EOF reached\n");
        exit(1);
    }

    return ((u2)high << 8) | (u2)low;
}

u4 read_u4(FILE* file) {
  u2 high = read_u2(file);
  u2 low = read_u2(file);

  return ((u4)high << 16) | (u4)low;
}

int main(int argc, const char **argv) {
  ClassFile class;

  if (argc != 2) {
    printf("Usage:\n  %s <.class>\n", argv[0]);
    return 1;
  }

  const char* filepath = argv[1];
  FILE *file =  fopen(filepath, "rb");

  if (file == NULL) {
    perror("fopen");
    return 1;
  }

  class.magic = read_u4(file);
  if (class.magic != (u4)MAGIC) {
    printf("File \"%s\" doesn't contain magic.\n"
        "  Expected: %X\n  Got: %X\n", filepath, MAGIC, class.magic);
    fclose(file);
    return 1;
  } 

  printf("Magic number %X correct!\n", class.magic);
  
  class.minor_version = read_u2(file);
  class.major_version = read_u2(file);

  printclass(&class, stdout);

  fclose(file);
  return 0;
}
