#include "reader.h"
#include "stdio.h"
#include "classfile.h"
#include "display.h"
#include "classfile_reader.h"
#include <stdlib.h>


int main(int argc, const char **argv) {
  ClassFile* class;

  if (argc != 2) {
    printf("Usage:\n  %s <.class>\n", argv[0]);
    return 1;
  }

  const char* filepath = argv[1];
  
  FILE* file = fopen(filepath, "rb");
  Reader reader = { NULL, 0 , 0};
  reader.buf = load_file(file, &reader.size);

  class = read_class(&reader);
  free(reader.buf);
  if (!class) return 1;

  //FILE* out = fopen("output.txt", "w");
  //printclass(class, out);

  printclass(class, stdout);
  free_classfile(class);

  fclose(file);
  //fclose(out);
  return 0;
}
