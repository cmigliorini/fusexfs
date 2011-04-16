/*
 * fuse_xfs.c
 * fuse-xfs
 *
 * Created by Alexandre Hardy on 4/16/11.
 * Copyright 2011 Nimbula. All rights reserved.
 *
 */

#include <fuse.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

static int
fuse_xfs_fgetattr(const char *path, struct stat *stbuf,
                  struct fuse_file_info *fi) {
  memset(stbuf, 0, sizeof(struct stat));
  
  if (strcmp(path, "/") == 0) { /* The root directory of our file system. */
    stbuf->st_mode = S_IFDIR | 0755;
    stbuf->st_nlink = 3;
    return 0;
  }
  return -ENOENT;
}

static int
fuse_xfs_getattr(const char *path, struct stat *stbuf) {
  return fuse_xfs_fgetattr(path, stbuf, NULL);
}

static int
fuse_xfs_readlink(const char *path, char *buf, size_t size) {
  return -ENOENT;
}

static int
fuse_xfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                 off_t offset, struct fuse_file_info *fi) {
  if (strcmp(path, "/") != 0) /* We only recognize the root directory. */
    return -ENOENT;
  
  filler(buf, ".", NULL, 0);           /* Current directory (.)  */
  filler(buf, "..", NULL, 0);          /* Parent directory (..)  */
  
  return 0;
}

static int
fuse_xfs_mknod(const char *path, mode_t mode, dev_t rdev) {
  return -ENOSYS;
}

static int
fuse_xfs_mkdir(const char *path, mode_t mode) {
  return -ENOSYS;
}

static int
fuse_xfs_unlink(const char *path) {
  return -ENOSYS;
}

static int
fuse_xfs_rmdir(const char *path) {
  return -ENOSYS;
}

static int
fuse_xfs_symlink(const char *from, const char *to) {
  return -ENOSYS;
}

static int
fuse_xfs_rename(const char *from, const char *to) {
  return -ENOSYS;
}

static int
fuse_xfs_exchange(const char *path1, const char *path2, unsigned long options) {
  return -ENOSYS;
}

static int
fuse_xfs_link(const char *from, const char *to) {
  return -ENOSYS;
}

static int
fuse_xfs_fsetattr_x(const char *path, struct setattr_x *attr,
                    struct fuse_file_info *fi) {
  return -ENOENT;
}

static int
fuse_xfs_setattr_x(const char *path, struct setattr_x *attr) {
  return -ENOENT;
}

static int
fuse_xfs_getxtimes(const char *path, struct timespec *bkuptime,
                   struct timespec *crtime) {
  return -ENOENT;
}

static int
fuse_xfs_create(const char *path, mode_t mode, struct fuse_file_info *fi) {
  return -ENOSYS;
}

static int
fuse_xfs_open(const char *path, struct fuse_file_info *fi) {
  return -ENOENT;
}

static int
fuse_xfs_read(const char *path, char *buf, size_t size, off_t offset,
              struct fuse_file_info *fi) {
  return -ENOSYS;
}

static int
fuse_xfs_write(const char *path, const char *buf, size_t size,
               off_t offset, struct fuse_file_info *fi) {
  return -ENOSYS;
}

static int
fuse_xfs_statfs(const char *path, struct statvfs *stbuf) {
  memset(stbuf, 0, sizeof(*stbuf));
  stbuf->f_files = 2;  /* For . and .. in the root */
  return 0;
}

static int
fuse_xfs_flush(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int
fuse_xfs_release(const char *path, struct fuse_file_info *fi) {
  return 0;
}

static int
fuse_xfs_fsync(const char *path, int isdatasync, struct fuse_file_info *fi) {
  return 0;
}

static int
fuse_xfs_setxattr(const char *path, const char *name, const char *value,
                  size_t size, int flags, uint32_t position) {
  return -ENOTSUP;
 }

static int
fuse_xfs_getxattr(const char *path, const char *name, char *value, size_t size,
                  uint32_t position) {
  return -ENOATTR;
}

static int
fuse_xfs_listxattr(const char *path, char *list, size_t size) {
  return 0;
}

static int
fuse_xfs_removexattr(const char *path, const char *name) {
  return -ENOATTR;
}

void *
fuse_xfs_init(struct fuse_conn_info *conn) {
  FUSE_ENABLE_XTIMES(conn);
  return NULL;
}

void
fuse_xfs_destroy(void *userdata) {
  /* nothing */
}

struct fuse_operations fuse_xfs_operations = {
  .init        = fuse_xfs_init,
  .destroy     = fuse_xfs_destroy,
  .getattr     = fuse_xfs_getattr,
  .fgetattr    = fuse_xfs_fgetattr,
/*  .access      = fuse_xfs_access, */
  .readlink    = fuse_xfs_readlink,
/*  .opendir     = fuse_xfs_opendir, */
  .readdir     = fuse_xfs_readdir,
/*  .releasedir  = fuse_xfs_releasedir, */
  .mknod       = fuse_xfs_mknod,
  .mkdir       = fuse_xfs_mkdir,
  .symlink     = fuse_xfs_symlink,
  .unlink      = fuse_xfs_unlink,
  .rmdir       = fuse_xfs_rmdir,
  .rename      = fuse_xfs_rename,
  .link        = fuse_xfs_link,
  .create      = fuse_xfs_create,
  .open        = fuse_xfs_open,
  .read        = fuse_xfs_read,
  .write       = fuse_xfs_write,
  .statfs      = fuse_xfs_statfs,
  .flush       = fuse_xfs_flush,
  .release     = fuse_xfs_release,
  .fsync       = fuse_xfs_fsync,
  .setxattr    = fuse_xfs_setxattr,
  .getxattr    = fuse_xfs_getxattr,
  .listxattr   = fuse_xfs_listxattr,
  .removexattr = fuse_xfs_removexattr,
  .exchange    = fuse_xfs_exchange,
  .getxtimes   = fuse_xfs_getxtimes,
  .setattr_x   = fuse_xfs_setattr_x,
  .fsetattr_x  = fuse_xfs_fsetattr_x,
};
