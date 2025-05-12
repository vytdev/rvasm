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

typedef unsigned long sloc_t; /* source loc */
typedef unsigned long rpos_t; /* position in output binary */
typedef unsigned long rsz_t;  /* raw size */

typedef enum {
  TK_NONE,
  TK_UNKNOWN,
  TK_EOF,
  TK_NEWLN,
  TK_OPNAME,
  TK_REG
} TokenType;

typedef struct {
  TokenType   tt;
  sloc_t   line, col, pos, len;
  char    *this_ln, *text, *fname;
} Token;

typedef struct {
  char     *src, *fname, *curr_ln;
  sloc_t   line, col, pos;
  int      end;
  Token    tok, lkahead;
} Lexer;

void lex_init (Lexer *l, char *src, char *fname);
int lex_isact (Lexer *l);
Token *lex_curr (Lexer *l);
Token *lex_next (Lexer *l);
Token *lex_peek (Lexer *l);

void print_token (Token *tok, char *fmt, ...);
signed int get_reg_idx (char *tok, int len);
signed int get_opcode (char *tok, int len);


typedef struct {
  Lexer  lex[MAXLSTCKSZ];
  int    top;
} LStack;

void lst_init (LStack *st);
void lst_free (LStack *st);
Lexer *lst_curr (LStack *st);
Lexer *lst_newf (LStack *st, char *fname, size_t nlen);
Lexer *lst_popf (LStack *st);


typedef enum {
  IR_INSTR
} IRType;

typedef struct {
  int     opc;
  char    rgA;
  char    rgB;
  char    rgC;
} IRInst;

typedef struct {
  IRType type;
  rpos_t  loc;
  rsz_t   size;
  union {
    IRInst i;
  } val;
} IRNode;

typedef struct {
  IRNode *ls;
  size_t alloc;
  size_t pos;
} IRList;

int ir_init (IRList *ir); /* 0 if failed */
void ir_free (IRList *ir);
IRNode *ir_push (IRList *ir);

IRList rvasm_parse (char *path);

#endif /* RVASM_H_ */
