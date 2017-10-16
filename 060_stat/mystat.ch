@x
At this point, I only need to process |argv[1]| in the |main| function.
@y
In Step~1, I wrote a main function that only needs to process |argv[1]|.

In Step~5, README says
{\cite
 Up to this point, your program has taken ONE command line argument,
 however, the real stat program takes an arbitrary number of arguments,
 and stats each one.  For example, you can run \.{stat README . /dev/null}
 and it will print the information for all three files one after
 the other.

}

So finally the |main| function here is able to handle multiple arguments.
@z

@x
int main(int argc, char **argv)
{
@y
int main(int argc, char **argv)
{
  int i;
  int rc = EXIT_SUCCESS;
@z

@x
  if (printstat(argv[1]) != 0) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
@y
  for (i = 1; i < argc; i++) {
    if (printstat(argv[i]) != 0) {
      rc = EXIT_FAILURE;
      /* \.{stat} returns failure if any argument caused an error */
    }
  }
  return rc;
@z

@x
@ On the first line I only need to print the filename.

@<print formatted stats@>+=
  printf("  File: '%s'\n", filename);
@y
@ On the first line I only need to print the filename.

But (in step 7) when the file is a symbolic link,
I should also output what it links to.
@s ssize_t int

@<print formatted stats@>+=
  if (S_ISLNK(st.st_mode)) { /* file is a symbolic link */
    char buf[256];
    ssize_t len;

    len = readlink(filename, buf, 256);
    assert(len >= 0); /* negative length does not make sense */
    if (len > 255) {
      buf[255] = '\0';
    } else {
      buf[len] = '\0';
    }
    printf("  File: '%s' -> '%s'\n", filename, buf);
  } else {
    printf("  File: '%s'\n", filename);
  }
@z

@x
@ On the third line, I need to print `Device', `Inode', and `Links'.

@<print formatted stats@>+=
  printf("Device: %lxh/%lud\tInode: %-10lu  Links: %lu\n",@|
    st.st_dev, st.st_dev, st.st_ino, st.st_nlink);

@y
@ On the third line, I need to print `Device', `Inode', and `Links'.

Additionally (in step 6), another field called ``Device type''
will be appended to the third line if the file happens to be
a device.

@<print formatted stats@>+=
  printf("Device: %lxh/%lud\tInode: %-10lu",
    st.st_dev, st.st_dev, st.st_ino);
  if (S_ISCHR(st.st_mode) || S_ISBLK(st.st_mode)) {
    /* file is a device */
    printf("  Links: %-5lu Device type: %d,%d\n",@|
      st.st_nlink, major(st.st_rdev), minor(st.st_rdev));
  } else {
    printf("  Links: %lu\n", st.st_nlink);
  }
@z

