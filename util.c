#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "rvdis.h"

#define BUFFSZ 4096


char *read_bin_file (char *path, size_t *out_sz)
{
  FILE *fp = fopen(path, "rb");
  if (!fp)
    return NULL;
  /* get the file's size. */
  fseek(fp, 0, SEEK_END);
  size_t sz = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  *out_sz = sz;
  /* alloc mem to load the file. */
  char *mem = (char*)malloc(sz);
  if (!mem) {
    fclose(fp);
    return NULL;
  }
  /* buffered read. */
  size_t curr_pos = 0;
  while (sz) {
    size_t rdneed, rdgot;
    rdneed = sz > BUFFSZ ? BUFFSZ : sz;
    rdgot = fread(mem + curr_pos, 1, rdneed, fp);
    if (rdgot != rdneed) {
      free(mem);
      fclose(fp);
      return NULL;
    }
    curr_pos += rdgot;
    sz -= rdgot;
  }
  fclose(fp);
  return mem;
}
