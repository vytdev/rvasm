/*
 *  rvdis -- An RVM bytecode disassembler.
 *  Copyright (C) 2025  Vincent Yanzee J. Tan
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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


char *to_mnemonic (int op)
{
  switch (op) {
#define DEF(op, idx) case (idx): return #op ;
#include "rvm/opcodes.h"
#undef DEF
  default: return ".raw";
  }
}
