#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "rvdis.h"

int main (int argc, char **argv)
{
  if (argc != 2) {
    fprintf(stderr, ""
      "usage: %s FILE\n"
      , argv[0]);
    return 1;
  }

  /* read the file. */
  size_t sz = 0;
  char *mem = read_bin_file(argv[1], &sz);
  if (!mem) {
    fprintf(stderr, ""
        "%s: Could not read file: %s\n", argv[0], argv[1]);
    return 1;
  }

  /* tmp. */
  puts(mem);
  free(mem);
  return 0;
}
