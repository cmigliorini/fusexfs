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
#include <xfsutil.h>

xfs_mount_t *fuse_xfs_mp = NULL;

xfs_mount_t *current_xfs_mount() {
    return fuse_xfs_mp;
}

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

struct filler_info_struct {
    void *buf;
    fuse_fill_dir_t filler;
};

int fuse_xfs_filldir(void *filler_info, const char *name, int namelen, off_t offset, uint64_t inumber, unsigned flags) {
    int r;
    char dir_entry[256];
    xfs_inode_t *inode=NULL;    
    struct stat stbuf;
    struct stat *stats = NULL;
    struct filler_info_struct *filler_data = (struct filler_info_struct *) filler_info;
    
    memcpy(dir_entry, name, namelen);
    dir_entry[namelen] = '\0';
    if (libxfs_iget(current_xfs_mount(), NULL, inumber, 0, &inode, 0)) {
        return 0;
    }
    if (!xfs_stat(inode, &stbuf)) {
        stats = &stbuf;
    }
    r = filler_data->filler(filler_data->buf, dir_entry, stats, 0);
    libxfs_iput(inode, 0);
    return r;
}

static int
fuse_xfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
                 off_t offset, struct fuse_file_info *fi) {
    int r;
    struct filler_info_struct filler_info;
    xfs_inode_t *inode=NULL;
    
    r = find_path(current_xfs_mount(), path, &inode);
    if (r) {
        return -ENOENT;
    }
    
    filler_info.buf = buf;
    filler_info.filler = filler;
    xfs_readdir(inode, (void *)&filler_info, 1024000, &offset, fuse_xfs_filldir);
    libxfs_iput(inode, 0);
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
    xfs_mount_t *mount = current_xfs_mount();
    
    memset(stbuf, 0, sizeof(*stbuf));
    stbuf->f_bsize = mount->m_sb.sb_blocksize;
    stbuf->f_frsize = mount->m_sb.sb_blocksize;
    stbuf->f_blocks =  mount->m_sb.sb_dblocks;
    stbuf->f_bfree =  mount->m_sb.sb_fdblocks;
    stbuf->f_files = mount->m_maxicount;
    stbuf->f_ffree = mount->m_sb.sb_ifree;
    stbuf->f_favail = mount->m_sb.sb_ifree;
    stbuf->f_namemax = MAXNAMELEN;
    stbuf->f_fsid = *((unsigned long*)mount->m_sb.sb_uuid);
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
                  size_t size, int flags) {
    return -ENOTSUP;
 }

static int
fuse_xfs_getxattr(const char *path, const char *name, char *value, size_t size) {
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
    //FUSE_ENABLE_XTIMES(conn);
    char *source_name = "/Users/ah/xfs.raw";// "/dev/rdisk1s1";
    char *progname = "fuse-xfs";
    fuse_xfs_mp = mount_xfs(progname, source_name);
    return NULL;
}

void
fuse_xfs_destroy(void *userdata) {
    libxfs_umount(fuse_xfs_mp);
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
  //Not supported:  
  //.exchange    = fuse_xfs_exchange,
  //.getxtimes   = fuse_xfs_getxtimes,
  //.setattr_x   = fuse_xfs_setattr_x,
  //.fsetattr_x  = fuse_xfs_fsetattr_x,
};
