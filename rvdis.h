#ifndef RVDIS_H_
#define RVDIS_H_   1

#include <stddef.h> /* for size_t. */
#include <stdio.h>
#include "rvm/rvm.h"

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
