/*
 * Public declarations for WUFS data structures
 * (c) the Great Class of 2015, especially nathan m perry
 *
 * General note:
 *   All WUFS on-disk struct field names have two letter (sb, in, de) prefixes.
 */
#ifndef WUFS_FS_H
#define WUFS_FS_H
#include <linux/types.h>

/*
 * This random value identifies this as a WUFS filesystem.
 * If this were a production system, we'd add ourselves to the
 * <linux/magic.h> file.  You can get this information with a call to
 * statfs(2) with any file in the file system.
 */
#define WUFS_MAGIC	0xEEEF

/*
 * the WUFS_BLOCKSIZE should be a multiple of the BLOCK_SIZE found in fs.h
 * Currently, that's 1024, so we're cool.  Later, we may have to bump this
 * up, but it will make small devices inefficent:
 *   - wasted space in boot and super blocks
 *   - (wildly) excessive bitmap over allocation for small devices
 *   - general internal fragmentation in files (see Berkeley's ffs for sol'ns)
 */
#define WUFS_BLOCKSIZE	1024 	/* size of file system's logical blocks */

/*
 * The wufs cleanliness bits.
 */
#define WUFS_VALID_FS		0x0001		/* a clean machine */
#define WUFS_ERROR_FS		0x0002		/* wufs with errors */

/**
 * wufs_super_block:
 * WUFS super-block data on disk (logical block 1).
 * Notes:
 *  - no support for versions; perhaps high nibble of magic
 *  - block bitmap includes *all* blocks on disk, not just data
 */
struct wufs_super_block {
  __u16 sb_magic;		/* unique identifier for WUFS version */
  __u16 sb_state;		/* state of the file system */
  __u16 sb_blocks;		/* count of disk blocks */
  __u16 sb_first_block;		/* block number of the first data block */
  __u16 sb_inodes;		/* count of inodes */
  __u16 sb_imap_bcnt;		/* the size (in blocks) of the imap */
  __u16 sb_bmap_bcnt;		/* the size (in blocks) of the bmap */
  __u32 sb_max_fsize;		/* the maximum file size (in bytes) */
};

/*
 * wufs_inode:
 * The on-disk format of WUFS inodes.
 * Notes:
 *   - size of nlinks is sufficient, but not necessary.
 *   - location of the u32 field arranged on u32 boundary to avoid padding
 *   - all pointers are direct
 *   - time is taken to be last modification time
 */
#define WUFS_LINK_MAX	        255
#define WUFS_INODE_DIRECT 7
#define WUFS_INODE_INDIRECT 1
#define WUFS_INODE_PTR_CT (WUFS_INODE_DIRECT + WUFS_INODE_INDIRECT*WUFS_BPTRS_PER_BLOCK)
#define WUFS_INODE_BPTRS (WUFS_INODE_DIRECT+WUFS_INODE_INDIRECT)
#define WUFS_INODESIZE   32
#define WUFS_INODES_PER_BLOCK (WUFS_BLOCKSIZE/WUFS_INODESIZE)
#define WUFS_ROOT_INODE 1 /* asserted lba of root directory's inode */

struct wufs_inode {
  __u16 in_mode;		/* file mode */
  __u16 in_nlinks;		/* number of links */
  __u16 in_uid;			/* user id */
  __u16 in_gid;			/* group id */
  __u32 in_time;		/* file modification time */
  __u32 in_size;		/* file size (bytes) */
  /* 14 bytes used so far...*/
  __u16 in_block[WUFS_INODE_BPTRS]; /* index of data blocks */
  /* block logically fills to WUFS_INODESIZE (see below) */
};

/*
 * wufs_dir_entry:
 * Notes:
 *   - 30 character names will not be null terminated; you have been warned
 *   - the directory entry size should be a power of two
 */
#define WUFS_NAMELEN 30
#define WUFS_DIRENTSIZE	(WUFS_NAMELEN + 2)
#define WUFS_DIRENTS_PER_BLOCK (WUFS_BLOCKSIZE/WUFS_DIRENTSIZE)

struct wufs_dirent {
  __u16 de_ino;			/* inode of entry */
  char  de_name[WUFS_NAMELEN];	/* name of directory file (strncpy-able) */
};

/*
 * wufs block pointer
 * 
 */
#define WUFS_BPTRS_PER_BLOCK (WUFS_BLOCKSIZE/2)

struct wufs_bptr {
  __u16 bp_block[WUFS_BPTRS_PER_BLOCK];          /* address of block */
};

#endif /* WUFS_FS_H */
