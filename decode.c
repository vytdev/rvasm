#include <stdarg.h>
#include <stdio.h>
#include "rvdis.h"
#include "rvm/rvm.h"


void print_reg (int idx)
{
  if (idx == RVM_RSP) printf("sp");
  else printf("r%d", idx);
}


void print_func (rvm_inst_t fn_part)
{
  printf("#0x%x", fn_part);
}


void print_comment (const char *fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);
  printf("\t; ");
  vprintf(fmt, ap);
  va_end(ap);
}


void print_inst (unsigned long pc, rvm_inst_t i)
{
  int opc = RVM_OPC(i);
  printf("%08x    %-10s", i, to_mnemonic(opc));

  switch (opc) {
    case RVM_OP_nop:
      break;
    case RVM_OP_mov:
      print_rgA(i);
      comma();
      print_rgB(i);
      break;
    case RVM_OP_trap:
      print_func(RVM_FNC(i) & 0xff);
      print_comment("%d", RVM_FNC(i) & 0xff);
      break;
    case RVM_OP_li:
      print_rgA(i);
      comma();
      print_f19(i);
      print_comment("%d", (int)
          RVM_SGXTD(RVM_FNC(i) & RVM_F19MASK, 19));
      break;
    case RVM_OP_j:
      print_f23(i);
      print_jmp(i);
      break;
    case RVM_OP_cmp:
      print_rgA(i);
      comma();
      print_rgB(i);
      break;
  }

  putc('\n', stdout);
}
