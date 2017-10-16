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

% vim: ts=2:sw=2:et:si
