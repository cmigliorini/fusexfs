#pragma once
#include <libc.h>
#include <xfs/libxfs.h>
#include <sys/stat.h>

typedef struct xfs_file_handle {
    // inode for file
    xfs_inode_t *inode;
    // current extent number
    xfs_extnum_t current_extent;
    // maximum number of extents
    xfs_extnum_t number_of_extents;
    // file size
    xfs_fsize_t size;
    // current offset in file
    off_t offset;
    // cached data fork
    xfs_ifork_t *dp;
    // cached extent record
    xfs_bmbt_irec_t extent_record;
    // cached mount point
    xfs_mount_t *mp;
} xfs_file_handle_t;

int
xfs_readdir(
            xfs_inode_t	*dp,
            void		*dirent,
            size_t		bufsize,
            xfs_off_t	*offset,
            filldir_t	filldir);

int xfs_readfile(xfs_inode_t *ip, void *buffer, off_t offset, size_t len, int *last_extent);
int xfs_readlink(xfs_inode_t *ip, void *buffer, off_t offset, size_t len, int *last_extent);
int xfs_stat(xfs_inode_t *inode, struct stat *stats);
int xfs_is_dir(xfs_inode_t *inode);
int xfs_is_link(xfs_inode_t *inode);
int xfs_is_regular(xfs_inode_t *inode);

int find_path(xfs_mount_t *mp, const char *path, xfs_inode_t **result);
xfs_mount_t *mount_xfs(char *progname, char *source_name);

struct xfs_name first_name(const char *path);
struct xfs_name next_name(struct xfs_name current);

int xfs_open(xfs_mount_t *mp, const char *path, xfs_file_handle_t *handle);
int xfs_close(xfs_file_handle_t *handle);
int xfs_read(xfs_file_handle_t *handle, void *buffer, off_t offset, size_t len);
int xfs_seek(xfs_file_handle_t *handle, off_t offset);
