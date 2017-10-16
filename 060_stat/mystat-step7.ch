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

% vim:ts=2:sw=2:et:si
