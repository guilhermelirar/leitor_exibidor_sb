#include "stdio.h"
#include "classfile.h"
#include "display.h"
#include "classfile_reader.h"


int main(int argc, const char **argv) {
  ClassFile* class;

  if (argc != 2) {
    printf("Usage:\n  %s <.class>\n", argv[0]);
    return 1;
  }

  const char* filepath = argv[1];
  
  class = load_class(filepath);
  if (!class) return 1;

  printclass(class, stdout);
  for (int i = 0; i < class->methods_count; i++) {
    disasasm_method(class, i, stdout);
  }

  free_classfile(class);

  return 0;
}
