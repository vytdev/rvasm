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
  for (unsigned long i = 0; i < inst_num; i++)
    print_inst(i+1, insts[i]);

  free(mem);
  return 1;
}


int main (int argc, char **argv)
{
  if (argc < 2) {
    fprintf(stderr, ""
      "usage: %s FILE...\n"
      RVM_LABEL " Bytecode Disassembler\n"
      "Copyright (C) 2025  Vincent Yanzee J. Tan\n"
      "This program is licensed under the GNU General Public\n"
      "License v3 or later. See <https://www.gnu.org/licenses/>\n"
      "for details.\n"
      , argv[0]);
    return 1;
  }

  for (int i = 1; i < argc; i++)
    disas_file(argv[0], argv[i]);

  return 0;
}
