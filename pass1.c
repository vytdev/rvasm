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

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "rvasm.h"


IRNode *ir_push (void)
{
  IRNode *node = (IRNode*)alloc(sizeof(IRNode));
  static IRNode *ir_head = NULL, *ir_tail = NULL;
  node->next = NULL;
  if (!ir_head)
    ir_head = node;
  if (ir_tail)
    ir_tail->next = node;
  ir_tail = node;
  return node;
}


int rvasm_parse (char *path)
{
  Lexer *l = NULL;
  l = lst_newf(path, strlen(path));
  if (!l) {
    printf("Could not load file: %s\n", path);
    return 0;
  }
  /* print all tokens. */
  while (lex_isact(l)) {
    Token *tok;
    tok = lex_next(l);
    print_token(tok, "tok: %d\n", tok->tt);
  }
  lst_free();
  return 1;
}
