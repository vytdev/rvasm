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
#include <stdlib.h>
#include <string.h>

#include "rvasm.h"


int ir_init (IRList *ir)
{
  ir->ls = (IRNode*)malloc(sizeof(IRNode) * 1024);
  if (!ir->ls)
    return 0;
  ir->pos = 0;
  ir->alloc = 1024;
  return 1;
}


void ir_free (IRList *ir)
{
  if (ir->ls)
    free(ir->ls);
  ir->ls = NULL;
  ir->alloc = 0;
  ir->pos = 0;
}


static int ir_resize (IRList *ir)
{
  IRNode *newls;
  size_t newsz = ir->alloc * 2;
  newls = (IRNode*)realloc(ir->ls, sizeof(IRNode) * newsz);
  if (!newls)
    return 0;
  ir->ls = newls;
  ir->alloc = newsz;
  return 1;
}


IRNode *ir_push (IRList *ir)
{
  if (ir->alloc >= ir->pos)
    if (!ir_resize(ir))
      return NULL;
  return &ir->ls[ir->pos++];
}


IRList rvasm_parse (char *path)
{
  IRList ir;
  LStack st;
  Lexer *l;
  ir.ls = NULL;

  lst_init(&st);
  l = lst_newf(&st, path, strlen(path));
  if (!l) {
    printf("Could not load file: %s\n", path);
    return ir;
  }

  /* print all tokens. */
  while (lex_isact(l)) {
    Token *tok;
    tok = lex_next(l);
    print_token(tok, "tok: %d\n", tok->tt);
  }

  lst_free(&st);
  return ir;
}
