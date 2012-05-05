/*
 * main.c
 * fuse-xfs
 *
 * Created by Alexandre Hardy on 4/16/11.
 *
 */
#include "fuse.h"
#include "fuse_xfs.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <xfsutil.h>

#define RAW_SECTOR_SIZE 512

extern struct fuse_operations fuse_xfs_operations;

void usage(int argc, char *argv[]) {
    fprintf(stderr, "fuse-xfs device/file [-p] [-- fuse-opts]\n");
    fprintf(stderr, "         [-p]     Only probe if the device contains an XFS filesystem.\n");
    fprintf(stderr, "         --       All options after -- are passed on to fuse.\n");
    fprintf(stderr, "                  The mount point must be supplied as the first argument.\n");
}

int parse_options(struct fuse_xfs_options* opts, int argc, char *argv[], int *new_argc, char *new_argv[]) {
    int i;
    
    if (argc < 2) {
        return 0;
    }
    
    opts->device = argv[1];
    
    new_argv[0] = argv[0];
        
    for (i=2; i<argc && strcmp(argv[i], "--"); i++) {
        if (!strcmp(argv[i], "-p")) {
            opts->probeonly = 1;
        }
        if (!strcmp(argv[i], "-l")) {
            opts->printlabel = 1;
        }
        if (!strcmp(argv[i], "-u")) {
            opts->printuuid = 1;
        }
    }
    
    *new_argc = 1;
    i++;
    for (; i<argc; i++) {
        new_argv[*new_argc] = argv[i];
        (*new_argc)++;
    }
    
    return 1;
}

int xfs_probe(struct fuse_xfs_options* opts) {
    int status, fd;
    char magic[RAW_SECTOR_SIZE+1];
    struct stat statbuf;
    xfs_mount_t *fuse_xfs_mp;

    status = stat(opts->device, &statbuf);
    if (status != 0) {
        fprintf(stderr, "Failed to open %s\n", opts->device);
        return 0;
    }
    
    fd = open(opts->device, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device");
        fprintf(stderr, "Failed to open %s for reading\n", opts->device);
        return 0;
    }
    
    status = read(fd, magic, RAW_SECTOR_SIZE);
    if (status != RAW_SECTOR_SIZE) {
        perror("Failure reading signature");
        fprintf(stderr, "Failed to read XFS signature on %s\n", opts->device);
        close(fd);
        return 0;
    }
    
    close(fd);
    
    if (strncmp(magic, "XFSB", 4)) {
        fprintf(stderr, "No XFS signature on %s\n", opts->device);
        close(fd);
        return 0;
    }
    
    fuse_xfs_mp = mount_xfs("probe", opts->device);
    if (fuse_xfs_mp == NULL) {
        fprintf(stderr, "%s doesn't appear to have a valid XFS filesystem\n", opts->device);
        return 0;
    }
    
    opts->xfs_mount = fuse_xfs_mp;
    return 1;
}

int main(int argc, char* argv[], char* envp[], char** exec_path) {
    struct fuse_xfs_options opts;
    char *fuse_argv[20];
    int fuse_argc = argc;
    
    memset(&opts, 0, sizeof(opts));
    
    /* All arguments for xfs must precede fuse arguments */
    if (!parse_options(&opts, argc, argv, &fuse_argc, fuse_argv)) {
        usage(argc, argv);
        return 1;
    }
    
    if (!xfs_probe(&opts)) {
        return 2;
    }
    
    if (opts.probeonly) {
        libxfs_umount(opts.xfs_mount);
        return 0;
    }

    if (opts.printlabel) {
        printf("%s\n", opts.xfs_mount.m_name); 
        libxfs_umount(opts.xfs_mount);
        return 0;
    }

    if (opts.printuuid) {
        printf("%s\n", opts.xfs_mount.m_sb.sb_uuid); 
        libxfs_umount(opts.xfs_mount);
        return 0;
    }
        
    return fuse_main(fuse_argc, fuse_argv, &fuse_xfs_operations, &opts);
}
