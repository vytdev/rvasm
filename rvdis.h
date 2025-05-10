#ifndef RVDIS_H_
#define RVDIS_H_   1

#include <stddef.h> /* for size_t. */
#include <stdio.h>
#include "rvm/rvm.h"


#define print_rgA(i) (print_reg(RVM_RGA((i))))
#define print_rgB(i) (print_reg(RVM_RGB((i))))
#define print_rgC(i) (print_reg(RVM_RGC((i))))

#define print_f11(i) (print_func(RVM_FNC((i)) & RVM_F11MASK))
#define print_f15(i) (print_func(RVM_FNC((i)) & RVM_F15MASK))
#define print_f19(i) (print_func(RVM_FNC((i)) & RVM_F19MASK))
#define print_f23(i) (print_func(RVM_FNC((i)) & RVM_F23MASK))

#define comma() (printf(", "))
#define print_jmp(i) (print_comment("<%lx>", pc + \
          RVM_SGXTD(RVM_FNC((i)) & RVM_F23MASK, 23)))

/*
 * Loads a binary file.
 */
char *read_bin_file (char *path, size_t *out_sz);

/*
 * Opcode to readable mnemonic.
 */
char *to_mnemonic (int op);

/*
 * Prints a decoded instruction.
 */
void print_inst (unsigned long pc, rvm_inst_t i);

/*
 * Print register.
 */
void print_reg (int idx);

/*
 * Print func bits.
 */
void print_func (rvm_inst_t fn_part);

/*
 * Print comment.
 */
void print_comment (const char *fmt, ...);

#endif /* RVDIS_H_ */
