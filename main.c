#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "rvdis.h"


int disas_file (char *prog, char *path)
{
  /* read the file. */
  size_t sz = 0;
  char *mem = read_bin_file(path, &sz);
  if (!mem) {
    fprintf(stderr, ""
        "%s: Could not read file: %s\n", prog, path);
    return 1;
  }

  printf("\nDisassembly of file:    %s\n\n", path);

  rvm_inst_t *insts = (rvm_inst_t*)(void*)mem;
  unsigned long inst_num = sz >> 2;
  for (unsigned long i = 0; i < inst_num; i++) {
    printf(" %6lx:    ", i << 2);
    print_inst(i+1, insts[i]);
  }

  free(mem);
  return 1;
}


int main (int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, ""
      "usage: %s FILE...\n"
      , argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++)
    disas_file(argv[0], argv[i]);

  return 0;
}
