#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//This function is used to figure out the ordering
//of the strings in qsort.  You do not need
//to modify it.
int stringOrder(const void * vp1, const void * vp2) {
  const char * const * p1 = vp1;
  const char * const * p2 = vp2;
  return strcmp(*p1, *p2);
}
//This function will sort and print data (whose length is count).
void sortData(char ** data, size_t count) {
  qsort(data, count, sizeof(char *), stringOrder);
}

void redirect(const char *filename)
{
  if (!freopen(filename, "r", stdin)) {
    fprintf(stderr, "failed to open input\n");
    exit(EXIT_FAILURE);
  }
}

#define INITLINES 256

void doSort(void)
{
  char **lines;
  size_t lines_size, lines_cap, i;

  lines_size = 0;
  lines_cap = INITLINES;
  lines = malloc(lines_cap * sizeof (char *));
  if (!lines) goto OOM;

  /* read */
  for (;;) {
    char *l = 0;
    size_t l_size = 0;
    if (getline(&l, &l_size, stdin) == -1) {
      free(l); // need to free it anyway (according to man page)
      if (feof(stdin)) {
        break; // good
      }
      // otherwise bad
      fprintf(stderr, "getline failed\n");
      /* note: not expected to free all the memory */
      exit(EXIT_FAILURE);
    }
    if (lines_size == lines_cap) {
      if (lines_cap * 2 < lines_cap) {
        /* we have a problem of overflow,
         * try a modest increment. */
        lines_cap++;
        if (lines_cap == 0) {
          /* I don't think this can happen before OOM */
          fprintf(stderr, "size_t overflow.\n");
          exit(EXIT_FAILURE);
        }
      } else {
        lines_cap *= 2;
      }
      lines = realloc(l, lines_cap * sizeof (char *));
      if (lines == 0) goto OOM;
    }
    lines[lines_size++] = l;
  }

  /* sort */
  sortData(lines, lines_size);

  /* write */
  for (i = 0; i < lines_size; i++) {
    fputs(lines[i], stdout);
    free(lines[i]); //
  }                 // free
  free(lines);      //

  return;
OOM:
  perror("sortLines"); // hopefully this works when OOM
  exit(EXIT_FAILURE);
}

int main(int argc, char ** argv) {
  if (argc > 1) {
    int i;

    for (i = 1; i < argc; i++) {
      redirect(argv[i]);
      doSort();
    }
  } else {
    doSort();
  }

  if (fclose(stdin) != 0) {
    fprintf(stderr, "failed to close input\n");
    exit(EXIT_FAILURE);
  }
  
  return EXIT_SUCCESS;
}

// vim: ts=2:sw=2:et
