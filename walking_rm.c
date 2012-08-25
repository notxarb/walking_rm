/*************************************************************************\
*                  Copyright (C) Braxton Huggins, 2012.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU Affero General Public License as published   *
* by the Free Software Foundation, either version 3 or (at your option)   *
* any later version. This program is distributed without any warranty.    *
* See the file COPYING.agpl-v3 for details.                               *
\*************************************************************************/


/* walking_rm.c
  History: I ran into an instance where a client made a directory with many
  files in it. The folder no longer used the standard 4 kilobytes, but was
  using almost 1 gigabyte. Standard rm, ls, find and other utilities were
  using lots of ram on the server while trying to get the list of files in
  the directory. The design of this program is to walk through the directory
  and remove the files rather than get the list and remove them. This
  approach will take more time, but should not make memory usage explode.
  Total file count was around 18.7 million files. It did take a day and a
  half, or maybe 2 days for it to delete all those files, but it did so
  without using gigabytes of ram.
*/
#if defined(__sun)
#define _XOPEN_SOURCE 500       /* Solaris 8 needs it this way */
#else
#if ! defined(_XOPEN_SOURCE) || _XOPEN_SOURCE < 600
#define _XOPEN_SOURCE 600       /* Get nftw() and S_IFSOCK declarations */
#endif
#endif
#include <ftw.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,
                           plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */


// I know globals are bad
int count = 0;

static void usageError(const char *progName, const char *msg)
{
    if (msg != NULL)
        fprintf(stderr, "%s\n", msg);
    fprintf(stderr, "Usage: %s [-d] [-m] [-p] [directory-path]\n", progName);
    fprintf(stderr, "\t-d Use FTW_DEPTH flag\n");
    fprintf(stderr, "\t-m Use FTW_MOUNT flag\n");
    fprintf(stderr, "\t-p Use FTW_PHYS flag\n");
    exit(EXIT_FAILURE);
}

/* Function called by nftw() */
static int dirTree(const char *pathname, const struct stat *sbuf, int type,
        struct FTW *ftwb)
{
    count++;
    if (count % 1000 == 0)
      printf("%i\n", count);
    if (type == FTW_F)
    {
      printf("unlinking %s\n", &pathname[0]);
      unlink(&pathname[0]);
    }
    
    return 0; /* Tell nftw() to continue */
}

int main(int argc, char *argv[])
{
    int flags, opt;
    char *path;

    flags = 0;
    while ((opt = getopt(argc, argv, "dmp")) != -1) {
        switch (opt) {
        case 'd': flags |= FTW_DEPTH;   break;
        case 'm': flags |= FTW_MOUNT;   break;
        case 'p': flags |= FTW_PHYS;    break;
        default:  usageError(argv[0], NULL);
        }
    }

    if (argc > optind + 1)
        usageError(argv[0], NULL);
    
    // get the real path for the unlink
    path = realpath((argc > optind) ? argv[optind] : ".", NULL);
    
    if (nftw(path, dirTree, 10, flags) == -1) {
        perror("nftw");
        exit(EXIT_FAILURE);
    }
    
    printf("%i\n", count);
    
    // Free the malloc's
    free(path);
    
    exit(EXIT_SUCCESS);
}
