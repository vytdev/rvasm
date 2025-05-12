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

#ifndef RVASM_UTILS_H_
#define RVASM_UTILS_H_   1

#include <stddef.h> /* for size_t. */
#include <stdio.h>


#define BUFFSZ      (4096)
#define DEFARENASZ  (16384) /* 16K */

typedef struct Arena Arena;
struct Arena {
  Arena *next;
  size_t size;
  size_t pos;
  char mem[1];
};

/*
 * Creates a new arena allocator.
 */
Arena *arena_new (size_t sz);

/*
 * Free an entire arena.
 */
void arena_free (Arena *ar);

/*
 * Allocate from stack. Returns an 8-byte aligned block.
 */
void *arena_alloc (Arena *ar, size_t sz);

/*
 * Reads a raw binary file.
 */
char *read_bin_file (char *path, size_t *out_sz);

/*
 * Reads an ASCII text file. (ends with a NUL-terminator)
 */
char *read_ascii_file (char *path, size_t *out_sz);

/*
 * Buffered read.
 */
size_t buffed_read (char *buf, size_t sz, FILE *fp);

/*
 * Returns the file's size.
 */
size_t get_file_size (FILE *fp);

#endif /* RVASM_UTILS_H_ */
