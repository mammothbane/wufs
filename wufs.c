/*
 * Utility routines supporting the Williams Unary File System
 * (c) the Great Class of 2015, especially nathan m perry
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wufs_fs.h"
#include "wufs.h"

/*
 * Compute (rounding up) chunks of size chunkSize needed to hold size items.
 * Might be used to find out how many blocks we need for i inodes:
 *    fullChunks(i,WUFS_BLOCKSIZE/WUFS_INODESIZE);
 */
int fullChunks(int size, int chunkSize)
{
  return (size + (chunkSize-1))/chunkSize;
}

/*
 * Some bitfield manipulation functions
 * Accidentally deleted (*sigh*) by Duane.
 */

/*
 * Set the ith bit (zero origin) in bitfield f
 */
void setBit(__u8 *f, int i)
{
  int bit = i % 8, idx = i / 8;
  f[idx] |= 1 << bit;
}

/*
 * Clear the ith bit (zero origin) in bitfield f
 */
void clearBit(__u8 *f, int i)
{
  int bit = i % 8, idx = i / 8;
  f[idx] &= ~(1 << bit);
}

/*
 * Return the ith bit (zero origin) in bitfield field: 0 or 1
 */
int getBit(__u8 *field, int i)
{
  int bit = i % 8, idx = i / 8;
  return field[idx] & (1 << bit);
}

/*
 * Find the next bit (starting with i or, if -1, 0) set in field of n.
 * If none, return -1.
 */
int findNextSet(__u8 *f, int i, int n)
{
  for (i = (i > 0) ? i : 0; i < n*8; i++) {
    if (getBit(f, i)) return i;
  }
  return -1;
}

/*
 * Find the next bit (starting with i or, if -1, 0) clear in field of n.
 * If none, return -1.
 */
int findNextClear(__u8 *f, int i, int n)
{
  for (i = (i > 0) ? i : 0; i < n*8; i++) {
    if (!getBit(f, i)) return i;
  }
  return -1;
}
