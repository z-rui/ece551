@q hint: Vim has the syntax highligher for this file installed by default @>

% Sometimes I just cite the text in README.
% I make it narrower so it is more obvious.
\def\cite{\par\smallskip \leftskip=.5in \rightskip=.5in}

@* Introduction.
This is the source for assignment 060.
Here's some instructions from README:

{\cite
In this assignment, you will be producing your own, simplified version
of the UNIX utility \.{stat}, which provides information about a file.
\dots
Your ultimate
goal is to write a program called \.{mystat} which replicates the 
functionality of the \.{stat} command without any options (that is, 
you aren't going to do \.{-t}, \.{-c}, etc).  If you run \.{stat something}
and \.{mystat something} they should produce exactly the same output.

}

@ Below are some header files already included in \.{mystat.c}.
I may want to include more when needed.

@<headers@>=
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

@ And a |time2str| function is provided, too.

@<definitions@>=
//This function is for Step 4
char * time2str(const time_t * when, long ns) {
  char * ans = malloc(128 * sizeof(*ans));
  char temp1[64];
  char temp2[32];
  const struct tm * t= localtime(when);
  strftime(temp1,512,"%Y-%m-%d %H:%M:%S",t);  
  strftime(temp2,32,"%z",t);
  snprintf(ans,128,"%s.%09ld %s", temp1, ns, temp2);
  return ans;
}

@ The whole program will look like this:

@c
@<headers@>@;
@<definitions@>@;
@<main program@>@;

@* Step 1.  As in README:

{\cite
  Your first goal is to make your mystat program accept exactly ONE
  filename as a command line argument, and print out the first
  THREE lines of the output that \.{stat} would print for that file.

}

At this point, I only need to process |argv[1]| in the |main| function.

@<main program@>=
int main(int argc, char **argv)
{
  if (argc < 2) {
    /* \.{stat} is run without arguments */
    fprintf(stderr,@|
      "stat: missing operand\n"@|
      "Try 'stat --help' for more information.\n");
    return EXIT_FAILURE;
  }

  if (printstat(argv[1]) != 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}

@ Before proceeding, I think there are some headers very useful.

@<headers@>+=
#include <assert.h> /* |assert| */
#include <string.h> /* |strcpy|, |strerror|, etc. */
#include <errno.h>  /* |errno| */

@ |printstat| is the \.{stat} program for a single file.
For example, passing |"README"| to it might produce output like this:
{\smallskip\tt\settabs 4\columns
\+File: `README'\cr
\+Size: 2825&           Blocks: 8&      IO Block: 4096& regular file\cr
\+Device: fc00h/64512d& Inode: 801829&  Links: 1\cr
\smallskip}

\noindent{\bf Note}\quad
The README said I should use the fancy quotes ({\tt `'}).
However, this specification is later changed (see Piazza @@366),
and I should use ordinary quotes ({\tt \char13\char13}) instead.

@<definitions@>+=
int printstat(const char *filename)
{
  struct stat st;

  @<read stats into |st|@>@;
  @<print formatted stats@>@;
  return 0; /* no error happened */
}

@ I can make use of the |lstat| system call to read the stats
into a |struct stat|.

Before printing anything, I need to check if the call succeeded.
(Things like nonexistent file or permission denied might happen.)

@<read stats...@>=
  if (lstat(filename, &st) != 0) {
    /* |lstat| returns non-zero on error */
    fprintf(stderr, "stat: cannot stat '%s': %s\n",
      filename, strerror(errno));
    return errno;
    /* don't call |exit|; real \.{stat} program proceeds
       and deals with other files */
  }

@ On the first line I only need to print the filename.

@<print formatted stats@>+=
  printf("  File: '%s'\n", filename);

@ On the second line I need to print `Size', `Blocks', and `IO Block'.

@<print formatted stats@>+=
{
  const char *filetype = NULL;

  switch (st.st_mode & S_IFMT) {
    case S_IFBLK:  filetype = "block special file";@+ break;
    case S_IFCHR:  filetype = "character special file";@+ break;
    case S_IFDIR:  filetype = "directory";@+ break;
    case S_IFIFO:  filetype = "fifo";@+ break;
    case S_IFLNK:  filetype = "symbolic link";@+ break;
    case S_IFREG:  filetype = "regular file";@+ break;
    case S_IFSOCK: filetype = "socket";@+ break;
  }

  assert(filetype != NULL);
  /* none of the possibilities described in README! I'm confused\dots */

  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",@|
    st.st_size, st.st_blocks, st.st_blksize, filetype);
}

@ On the third line, I need to print `Device', `Inode', and `Links'.

@<print formatted stats@>+=
  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",@|
    st.st_dev, st.st_dev, st.st_ino, st.st_nlink);

@* Index.


% vim: ts=2:sw=2:et:si

