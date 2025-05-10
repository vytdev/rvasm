/*
 *  rvdis -- An RVM bytecode disassembler.
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
