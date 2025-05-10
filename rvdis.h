#ifndef RVDIS_H_
#define RVDIS_H_   1

#include <stddef.h> /* for size_t. */


/*
 * Loads a binary file.
 */
char *read_bin_file (char *path, size_t *out_sz);

#endif /* RVDIS_H_ */
