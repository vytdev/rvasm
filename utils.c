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

#include "utils.h"


Arena *arena_new (size_t sz)
{
  Arena *ar;
  if (sz == 0)
    sz = DEFARENASZ;
  ar = (Arena*)malloc(sizeof(Arena)-1 + sz);
  if (!ar)
    return NULL;
  ar->next = NULL;
  ar->pos = 0;
  ar->size = sz;
  return ar;
}


void arena_free (Arena *ar)
{
  if (!ar)
    return;
  arena_free(ar->next);
  free(ar);
}


void *arena_alloc (Arena *ar, size_t sz)
{
  if (!ar)
    return NULL;
  /* align to 8 bytes */
  ar->pos = (ar->pos + 7) & ~7;
  if (ar->size - ar->pos > sz) {
    void *ptr = &ar->mem[ar->pos];
    ar->pos += sz;
    return ptr;
  }
  /* need more space */
  if (!ar->next) {
    size_t nsz = ar->size << 1;
    if (sz > nsz)
      nsz = sz;
    ar->next = arena_new(nsz);
  }
  return arena_alloc(ar->next, sz);
}


char *read_bin_file (char *path, size_t *out_sz)
{
  FILE *fp;
  size_t sz;
  char *mem;
  fp = fopen(path, "rb");
  if (!fp)
    return NULL;
  sz = get_file_size(fp);
  if (out_sz)
    *out_sz = sz;
  /* alloc mem to load the file. */
  mem = (char*)malloc(sz);
  if (!mem) {
    fclose(fp);
    return NULL;
  }
  /* buffered read. */
  if (buffed_read(mem, sz, fp) != sz) {
    free(mem);
    fclose(fp);
    return NULL;
  }
  fclose(fp);
  return mem;
}


char *read_ascii_file (char *path, size_t *out_sz)
{
  FILE *fp;
  size_t sz;
  char *mem;
  fp = fopen(path, "rb");
  if (!fp)
    return NULL;
  sz = get_file_size(fp);
  if (out_sz)
    *out_sz = sz;
  /* alloc mem to load the file. +1 for NUL */
  mem = (char*)malloc(sz + 1);
  if (!mem) {
    fclose(fp);
    return NULL;
  }
  /* buffered read. */
  if (buffed_read(mem, sz, fp) != sz) {
    free(mem);
    fclose(fp);
    return NULL;
  }
  fclose(fp);
  mem[sz] = '\0';
  return mem;
}


size_t buffed_read (char *buf, size_t sz, FILE *fp)
{
  size_t curr_pos = 0;
  while (sz) {
    size_t rdneed, rdgot;
    rdneed = sz > BUFFSZ ? BUFFSZ : sz;
    rdgot = fread(buf + curr_pos, 1, rdneed, fp);
    if (rdgot != rdneed)
      return rdgot;
    curr_pos += rdgot;
    sz -= rdgot;
  }
  return curr_pos;
}


size_t get_file_size (FILE *fp)
{
  size_t saved_pos = ftell(fp);
  size_t fsize;
  fseek(fp, 0, SEEK_END);
  fsize = ftell(fp);
  fseek(fp, saved_pos, SEEK_SET);  /* restore pos */
  return fsize;
}
