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

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "rvasm.h"
#include "rvm/defs.h"

#define isid(c) (isalnum((c)) || (c) == '_')


void lex_init (struct Lexer *l, char *src, char *fname)
{
  l->pos = 0;
  l->line = 1;
  l->col = 1;
  l->src = src;
  l->fname = fname;
  l->curr_ln = src;
  l->tok.tt = TK_NONE;
  l->lkahead.tt = TK_NONE;
  l->end = 0;
}


static void inc (struct Lexer *l)
{
  switch (l->src[l->pos++]) {
  case '\t':
    l->col += TABSTOP - ((l->col-1) % TABSTOP);
    break;
  case '\n':
    l->line++;
    l->curr_ln = &l->src[l->pos];
    /* fallthrough */
  case '\r':
    l->col = 1;
    break;
  default:
    l->col++;
  }
}


static char nextc (struct Lexer *l)
{
  return l->src[l->pos];
}


/*
 * Process the next token.
 */
static struct Token tokenize (struct Lexer *l)
{
  struct Token tok;
  char c;
  if (l->end)
    return l->tok;
  tok.tt = TK_UNKNOWN;
  tok.fname = l->fname;

  for (;;) {
    c = nextc(l);
    tok.pos = l->pos;
    tok.line = l->line;
    tok.col = l->col;
    tok.text = &l->src[l->pos];
    tok.this_ln = l->curr_ln;
    tok.len = 0;

    /* eof */
    if (c == '\0') {
      inc(l);
      tok.tt = TK_EOF;
      tok.len = 1;
      l->end = 1;
      break;
    }

    /* line breaks */
    if (c == '\n') {
      inc(l);
      tok.tt = TK_NEWLN;
      tok.len = 1;
      break;
    }

    /* skip spaces (except line/cr) */
    if (isspace(c)) {
      while (isspace(c) && c != '\n') {
        inc(l);
        c = nextc(l);
      }
      continue;
    }

    /* skip comments */
    if (c == ';') {
      while (c != '\n' && c != '\0') {
        inc(l);
        c = nextc(l);
      }
      continue;
    }

    /* comma */
    if (c == ',') {
      inc(l);
      tok.tt = TK_COMMA;
      tok.len = 1;
      break;
    }

    /* op mnemonics and regs */
    if (isid(c)) {
      while (isid(c)) {
        inc(l);
        c = nextc(l);
        tok.len++;
      }
      if (get_reg_idx(tok.text, tok.len) != -1)
        tok.tt = TK_REG;
      else if (get_opcode(tok.text, tok.len) != -1)
        tok.tt = TK_OPNAME;
      else /* unknown */
        l->end = 1;
      break;
    }

    /* unknown */
    tok.len++;
    l->end = 1;
    break;
  }
  return tok;
}


int lex_isact (struct Lexer *l)
{
  return !l->end;
}


struct Token *lex_curr (struct Lexer *l)
{
  return &l->tok;
}


struct Token *lex_next (struct Lexer *l)
{
  if (l->lkahead.tt != TK_NONE) {
    l->tok = l->lkahead;
    l->lkahead.tt = TK_NONE;
  }
  else
    l->tok = tokenize(l);
  return &l->tok;
}


struct Token *lex_peek (struct Lexer *l)
{
  if (l->lkahead.tt != TK_NONE)
    l->lkahead = tokenize(l);
  return &l->lkahead;
}


void print_token (struct Token *tok, char *fmt, ...)
{
  int i, j;

  if (fmt) {
    va_list ap;
    printf("%s: ", tok->fname);
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
  }

  /* print the token text. */
  printf("%5lu | ", tok->line);
  for (i = 0; ; ) {
    char c = tok->this_ln[i];
    switch (c) {
      case '\t':
        j = TABSTOP - (tok->col % TABSTOP);
        for (; j > 0; j--)
          putc(' ', stdout);
        break;
      case '\n': case '\r': case '\0':
        goto stop;
      default:
        putc(c, stdout);
    }
    i++;
  }

  stop:
  /* print a marker indicating where the token is. */
  putc('\n', stdout);
  printf("      | ");
  for (i = 1; i < tok->col; i++)
    putc(' ', stdout);
  for (i = 0; i < tok->len; i++)
    putc('^', stdout);
  putc('\n', stdout);
}


signed int get_reg_idx (char *tok, int len)
{
  /* TODO: optimise this */
#define reg_case(t, v) \
  if (strncmp(tok, (t), len) == 0) \
    return (v);
  reg_case("r0",  RVM_R0);
  reg_case("r1",  RVM_R1);
  reg_case("r2",  RVM_R2);
  reg_case("r3",  RVM_R3);
  reg_case("r4",  RVM_R4);
  reg_case("r5",  RVM_R5);
  reg_case("r6",  RVM_R6);
  reg_case("r7",  RVM_R7);
  reg_case("r8",  RVM_R8);
  reg_case("r9",  RVM_R9);
  reg_case("r10", RVM_R10);
  reg_case("r11", RVM_R11);
  reg_case("r12", RVM_R12);
  reg_case("r13", RVM_R13);
  reg_case("r14", RVM_R14);
  reg_case("r15", RVM_R15);
  reg_case("sp",  RVM_RSP);
#undef reg_case
  return -1;
}


signed int get_opcode (char *tok, int len)
{
#define DEF(op, idx) \
  if (strncmp(tok, #op, len) == 0) \
    return (idx);
#include "rvm/opcodes.h"
#undef DEF
  return -1;
}
