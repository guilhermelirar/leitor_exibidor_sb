#include "stdio.h"
#include <stdint.h>

#define MAGIC 0xCAFEBABE

typedef uint8_t  u1;  
typedef uint16_t u2;
typedef uint32_t u4;

u4 get_magic(FILE* file) {
  u4 ans = 0;
  for (int i = 0; (i < 4) && !feof(file); i++) {
    ans <<= 8;
    ans |= fgetc(file);
  }
  return ans;
}

int main(int argc, const char **argv) {
  if (argc != 2) {
    printf("Usage:\n  %s <.class>\n", argv[0]);
    return 1;
  }

  const char* filepath = argv[1];
  FILE *file =  fopen(filepath, "r");

  if (file == NULL) {
    printf("File \"%s\" could not be open\n", filepath);
    return 1;
  }

  u4 magic = get_magic(file);
  if (magic != (u4)MAGIC) {
    printf("File \"%s\" doesn't contain magic.\n"
        "  Expected: %X\n  Got: %X\n", filepath, MAGIC, magic);
    fclose(file);
    return 1;
  } 

  printf("Magic number %X correct!\n", magic);
   

  fclose(file);
  return 0;
}
