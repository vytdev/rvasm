/*
 *  rvasm -- An assembler and disassembler for rvm.
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

#include <stdio.h>

#include "rvm/rvm.h"
#include "rvasm.h"

Arena *glob_mem = NULL;


/*
 * Main.
 */
int main (int argc, char **argv)
{
  if (argc < 2) {
    printf(""
      "usage: %s FILE...\n"
      RVM_LABEL " Bytecode Assembler\n"
      "Copyright (C) 2025  Vincent Yanzee J. Tan\n"
      "This program is licensed under the GNU General Public\n"
      "License v3 or later. See <https://www.gnu.org/licenses/>\n"
      "for details.\n"
      , argv[0]);
    return 1;
  }

  glob_mem = arena_new(0);

  rvasm_parse(argv[1]);

  arena_free(glob_mem);
  return 0;
}
