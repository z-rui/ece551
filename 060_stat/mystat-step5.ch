@x
@* Step 1.  As in README:
@y
@* Step 1 \char38\ 5.  As in README:
@z

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

% vim: ts=2:sw=2:et:si
