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

@s tm int
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
@<declarations@>@;
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

@s stat int
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

  assert(filetype != NULL);@^confused@>
  /* none of the possibilities described in README! I'm confused\dots */

  printf("  Size: %-10lu\tBlocks: %-10lu IO Block: %-6lu %s\n",@|
    st.st_size, st.st_blocks, st.st_blksize, filetype);
}

@ On the third line, I need to print `Device', `Inode', and `Links'.

@<print formatted stats@>+=
  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",@|
    st.st_dev, st.st_dev, st.st_ino, st.st_nlink);


@* Step 2.  Now I need to deal with the fourth line
in the formatted output.
To leave room for future extension,
I will not hard-code what to print here.

@<print formatted stats@>+=
  @<print the fields in the fourth formatted line@>@;
  printf("\n"); /* terminate the fourth line */

@ At this point, only the first field (``Access'') needs to be output
on the fourth line.
It consists of two parts.  The first is an octal code of the permissions.
I need to mask |st_mode| with |~S_IFMT| to get that code.
The second part is a human-readable description
(I will call it ``mode string'' in later descriptions),
@^mode string@>
which can be generated by |get_modestr|.

@d MODESTR_LEN 10 /* length of the mode string, not including |'\0'| */
@<print the fields in the fourth ...@>=
{
  char modestr[MODESTR_LEN+1]; /* ${}+1$ for |'\0'| */

  printf("Access: (%04o/%s)", st.st_mode & ~S_IFMT,
    get_modestr(modestr, st.st_mode));
}

@ |get_modestr| generates the mode string.
@^mode string@>

@<declarations@>+=
char *get_modestr(char *buf, int mode);
@ @<definitions@>+=
char *get_modestr(char *buf, int mode)
{
  @<write file type into |buf[0]|@>@;
  @<write permissions into |buf[1]|\dots|buf[MODESTR_LEN-1]|@>@;
  buf[MODESTR_LEN] = '\0'; /* terminate the string */
  return buf;
}

@ The first character in the mode string
@^mode string@>
will need a big switch
similar to the one in Step 1.
But some characters are different, so I cannot reuse the code.

@<write file type ...@>=
{
  char c = 0;
  switch (mode & S_IFMT) {
    case S_IFBLK:  c = 'b';@+ break;
    case S_IFCHR:  c = 'c';@+ break;
    case S_IFDIR:  c = 'd';@+ break;
    case S_IFIFO:  c = 'p';@+ break;
    case S_IFLNK:  c = 'l';@+ break;
    case S_IFREG:  c = '-';@+ break;
    case S_IFSOCK: c = 's';@+ break;
  }
  assert(c != 0); /* Again, I'm confused when run out of possibilities. */@^confused@>
  buf[0] = c; /* write it into |buf[0]| */
}

@ |get_permissions| unifies the formatting
of user, group and other permissions.

@<declarations@>+=
void get_permissions(char *buf, int mode, int rmask, int wmask, int xmask);
@ @<definitions@>+=
void get_permissions(char *buf, int mode, int rmask, int wmask, int xmask)
{
  buf[0] = (mode & rmask) ? 'r' : '-';
  buf[1] = (mode & wmask) ? 'w' : '-';
  buf[2] = (mode & xmask) ? 'x' : '-';
}

@ Now I can call |get_permissions| to fill in the corresponding fields
in the mode string.
@^mode string@>

@<write permissions ...@>=
  get_permissions(buf+1, mode, S_IRUSR, S_IWUSR, S_IXUSR);
  get_permissions(buf+4, mode, S_IRGRP, S_IWGRP, S_IXGRP);
  get_permissions(buf+7, mode, S_IROTH, S_IWOTH, S_IXOTH);


@* Step 3.  Now deal with the other fields in the fourth line.
They are ``Uid'' and ``Gid''.

@ First deal with ``Uid''.
I can use the |getpwuid| library function
to look up the user's name from it's user~ID.

Be careful about the leading spaces in the output.
They are here to separate the current field from
the previous one.

@s passwd int
@<print the fields in the fourth ...@>+=
{
  struct passwd *pw;

  pw = getpwuid(st.st_uid);
  printf("  Uid: (%5d/%8s)", st.st_uid, pw->pw_name);
}

@ Then ``Gid''.  This time use |getgrgid|.

@s group int
@<print the fields in the fourth ...@>+=
{
  struct group *gr;

  gr = getgrgid(st.st_gid);
  printf("   Gid: (%5d/%8s)", st.st_gid, gr->gr_name);
}


@* Step 4.
In this step I will add four more lines to the output.
They are the last ones, and are related to time information.

Again, these lines have some similarities,
so I will put the actual printing code
into a separate function |print_timeinfo|.

@<print formatted stats@>+=
  print_timeinfo("Access", &st.st_atim);
  print_timeinfo("Modify", &st.st_mtim);
  print_timeinfo("Change", &st.st_ctim);
  printf(" Birth: -\n"); /* ``Birth'' line is always this */

@ @s timespec int
@<declarations@>+=
void print_timeinfo(const char *what, const struct timespec *when);

@ |print_timeinfo| uses the provided |time2str| function
to format the time value into a string.
@<definitions@>+=
void print_timeinfo(const char *what, const struct timespec *when)
{
  char *timestr;
  
  timestr = time2str(&when->tv_sec, when->tv_nsec);
  printf("%s: %s\n", what, timestr);
  free(timestr);
}


@* Step 5, 6, 7.
These steps require modifications to previous steps.

The modifications are already in place.
So no code will be shown here.

An asterisk next to a section number indicates modification.


@* Index.  Here is the index of all C identifiers, in alphabetical order.
The listed numbers are section numbers.
The underlined ones are where the corresponding identifiers are defined.


% vim: ts=2:sw=2:et:si
