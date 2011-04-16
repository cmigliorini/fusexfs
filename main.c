/*
 * main.c
 * fuse-xfs
 *
 * Created by Alexandre Hardy on 4/16/11.
 * Copyright 2011 Nimbula. All rights reserved.
 *
 * Compile on the command line as follows:
 * gcc -o "fuse-xfs" fuse_xfs.c main.c -lfuse
 *     -D_FILE_OFFSET_BITS=64 -D__FreeBSD__=10 -DFUSE_USE_VERSION=26
 */
#include "fuse.h"

extern struct fuse_operations fuse_xfs_operations;

int main(int argc, char* argv[], char* envp[], char** exec_path) {
  umask(0);
  return fuse_main(argc, argv, &fuse_xfs_operations, NULL);
}
