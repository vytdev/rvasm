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

#ifndef RVASM_H_
#define RVASM_H_   1

#include <stddef.h>


#define TABSTOP     (4)
#define MAXLSTCKSZ  (48)

typedef unsigned long sloc;

enum TokenType {
  TK_NONE,
  TK_UNKNOWN,
  TK_EOF,
  TK_NEWLN,
  TK_COMMA,
  TK_OPNAME,
  TK_REG
};

struct Token {
  enum TokenType   tt;
  sloc     line, col, pos, len;
  char    *this_ln, *text, *fname;
};

struct Lexer {
  char     *src, *fname, *curr_ln;
  sloc     line, col, pos;
  int      end;
  struct Token tok, lkahead;
};

struct LStack {
  struct Lexer lex[MAXLSTCKSZ];
  int    top;
};

void lex_init (struct Lexer *l, char *src, char *fname);
int lex_isact (struct Lexer *l);
struct Token *lex_curr (struct Lexer *l);
struct Token *lex_next (struct Lexer *l);
struct Token *lex_peek (struct Lexer *l);

void print_token (struct Token *tok, char *fmt, ...);
signed int get_reg_idx (char *tok, int len);
signed int get_opcode (char *tok, int len);

void lst_init (struct LStack *st);
void lst_free (struct LStack *st);
struct Lexer *lst_curr (struct LStack *st);
struct Lexer *lst_newf (struct LStack *st, char *fname, size_t nlen);
struct Lexer *lst_popf (struct LStack *st);

#endif /* RVASM_H_ */
