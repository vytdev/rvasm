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

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "rvm/rvm.h"

#define BUFFSZ 4096


/*
 * Opcode to readabl mnemonic.
 */
char *to_mnemonic (int op)
{
  switch (op) {
#define DEF(op, idx) case (idx): return #op ;
#include "rvm/opcodes.h"
#undef DEF
  default: return ".raw";
  }
}


/*
 * Print register text.
 */
void print_reg (int idx)
{
  if (idx == RVM_RSP) printf("sp");
  else printf("r%d", idx);
}


/*
 * Print func bits as hex.
 */
void print_func (rvm_inst_t fn_part)
{
  printf("#0x%x", fn_part);
}


/*
 * Print a comment for annotation.
 */
void print_comment (const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  printf("\t\t; ");
  vprintf(fmt, ap);
  va_end(ap);
}


#define print_rgA() (print_reg(RVM_RGA(i)))
#define print_rgB() (print_reg(RVM_RGB(i)))
#define print_rgC() (print_reg(RVM_RGC(i)))

#define print_f11() (print_func(RVM_FNC(i) & RVM_F11MASK))
#define print_f15() (print_func(RVM_FNC(i) & RVM_F15MASK))
#define print_f19() (print_func(RVM_FNC(i) & RVM_F19MASK))
#define print_f23() (print_func(RVM_FNC(i) & RVM_F23MASK))

#define comma() (printf(", "))

#define print_imm11s() (print_comment("%d", (int) \
          RVM_SGXTD(RVM_FNC(i) & RVM_F11MASK, 11)))
#define print_imm15s() (print_comment("%d", (int) \
          RVM_SGXTD(RVM_FNC(i) & RVM_F15MASK, 15)))
#define print_imm19s() (print_comment("%d", (int) \
          RVM_SGXTD(RVM_FNC(i) & RVM_F19MASK, 19)))
#define print_imm23s() (print_comment("%d", (int) \
          RVM_SGXTD(RVM_FNC(i) & RVM_F23MASK, 23)))

#define print_imm11u() (print_comment("%u", (int) \
          RVM_ZRXTD(RVM_FNC(i) & RVM_F11MASK, 11)))
#define print_imm15u() (print_comment("%u", (int) \
          RVM_ZRXTD(RVM_FNC(i) & RVM_F15MASK, 15)))
#define print_imm19u() (print_comment("%u", (int) \
          RVM_ZRXTD(RVM_FNC(i) & RVM_F19MASK, 19)))
#define print_imm23u() (print_comment("%u", (int) \
          RVM_ZRXTD(RVM_FNC(i) & RVM_F23MASK, 23)))


/*
 * Print an instruction.
 */
void print_inst (unsigned long pc, rvm_inst_t i)
{
  int opc = RVM_OPC(i);
  printf(" %6lx:    ", (pc-1) << 2);
  printf("%08x    %-10s", i, to_mnemonic(opc));

  switch (opc) {
    case RVM_OP_nop:
    case RVM_OP_ret:
      break;

    case RVM_OP_mov:
    case RVM_OP_cmp:
    case RVM_OP_cpl:
    case RVM_OP_neg:
    case RVM_OP_swp:
      print_rgA();
      comma();
      print_rgB();
      break;

    case RVM_OP_trap:
      print_func(RVM_FNC(i) & 0xff);
      print_comment("%d", RVM_FNC(i) & 0xff);
      break;

    case RVM_OP_li:
    case RVM_OP_cmpi:
      print_rgA();
      comma();
      print_f19();
      print_imm19s();
      break;

    case RVM_OP_adr:
    case RVM_OP_loop:
      print_rgA();
      comma();
      printf("%lx", (pc +
          RVM_SGXTD(RVM_FNC(i) & RVM_F19MASK, 19)) << 2);
      break;

    case RVM_OP_j:
    case RVM_OP_je:
    case RVM_OP_jne:
    case RVM_OP_jg:
    case RVM_OP_ja:
    case RVM_OP_jl:
    case RVM_OP_jb:
    case RVM_OP_jge:
    case RVM_OP_jae:
    case RVM_OP_jle:
    case RVM_OP_jbe:
    case RVM_OP_call:
      printf("%lx", (pc + \
          RVM_SGXTD(RVM_FNC(i) & RVM_F23MASK, 23)) << 2);
      break;

    case RVM_OP_inc:
    case RVM_OP_dec:
    case RVM_OP_jr:
    case RVM_OP_callr:
      print_rgA();
      break;

    case RVM_OP_add:
    case RVM_OP_sub:
    case RVM_OP_mul:
    case RVM_OP_div:
    case RVM_OP_mod:
    case RVM_OP_muls:
    case RVM_OP_divs:
    case RVM_OP_and:
    case RVM_OP_orr:
    case RVM_OP_xor:
    case RVM_OP_shl:
    case RVM_OP_shr:
      print_rgA();
      comma();
      print_rgB();
      comma();
      print_rgC();
      break;

    case RVM_OP_addi:
    case RVM_OP_subi:
    case RVM_OP_muli:
    case RVM_OP_divi:
    case RVM_OP_modi:
    case RVM_OP_mulsi:
    case RVM_OP_divsi:
    case RVM_OP_andi:
    case RVM_OP_orri:
    case RVM_OP_xori:
    case RVM_OP_shli:
    case RVM_OP_shri:
      print_rgA();
      comma();
      print_rgB();
      comma();
      print_f15();
      break;

    case RVM_OP_rd8:
    case RVM_OP_wr8:
    case RVM_OP_rd16:
    case RVM_OP_wr16:
    case RVM_OP_rd32:
    case RVM_OP_wr32:
    case RVM_OP_rd64:
    case RVM_OP_wr64:
      print_rgA();
      comma();
      putc('[', stdout);
      print_rgB();
      printf(" + #%d]",
         (int)RVM_SGXTD(RVM_FNC(i) & RVM_F15MASK, 15));
      break;
  }

  putc('\n', stdout);
}


/*
 * Read binary file.
 */
char *read_bin_file (char *path, size_t *out_sz)
{
  FILE *fp;
  size_t sz;
  char *mem;
  size_t curr_pos;
  fp = fopen(path, "rb");
  if (!fp)
    return NULL;
  /* get the file's size. */
  fseek(fp, 0, SEEK_END);
  sz = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  *out_sz = sz;
  /* alloc mem to load the file. */
  mem = (char*)malloc(sz);
  if (!mem) {
    fclose(fp);
    return NULL;
  }
  /* buffered read. */
  curr_pos = 0;
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


/*
 * Disassemble a binary file.
 */
int disas_file (char *prog, char *path)
{
  /* read the file. */
  size_t sz = 0;
  rvm_inst_t *insts;
  unsigned long inst_num, pc;
  char *mem = read_bin_file(path, &sz);
  if (!mem) {
    printf("%s: Could not read file: %s\n\n", prog, path);
    return 1;
  }
  printf("Disassembly of file:    %s\n\n", path);
  insts = (rvm_inst_t*)(void*)mem;
  inst_num = sz >> 2;
  for (pc = 0; pc < inst_num; pc++)
    print_inst(pc+1, insts[pc]);
  putc('\n', stdout);
  free(mem);
  return 1;
}


/*
 * Main.
 */
int main (int argc, char **argv)
{
  int i;
  if (argc < 2) {
    printf(""
      "usage: %s FILE...\n"
      RVM_LABEL " Bytecode Disassembler\n"
      "Copyright (C) 2025  Vincent Yanzee J. Tan\n"
      "This program is licensed under the GNU General Public\n"
      "License v3 or later. See <https://www.gnu.org/licenses/>\n"
      "for details.\n"
      , argv[0]);
    return 1;
  }
  for (i = 1; i < argc; i++)
    disas_file(argv[0], argv[i]);
  return 0;
}
