#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "counts.h"

static void errorExit(void)
{
  perror("counts");
  exit(EXIT_FAILURE);
}

counts_t * createCounts(void) {
  counts_t *c;

  c = malloc(sizeof *c);
  if (c == 0) {
    errorExit();
  }
  c->base = NULL;
  c->len = 0;
  c->unknowns = 0;
  return c;
}

void addCount(counts_t * c, const char * name) {
  size_t i;
  one_count_t *p;

  assert(c != NULL);

  if (name == NULL) {
    c->unknowns++;
    return;
  }
  for (i = 0; i < c->len; i++) {
    p = &c->base[i];
    if (strcmp(p->key, name) == 0) {
      break;
    }
  }
  if (i == c->len) {
    c->base = realloc(c->base, (c->len + 1) * sizeof c->base[0]);
    if (c->base == NULL) {
      errorExit();
    }
    p = &c->base[c->len++];
    p->key = strdup(name);
    if (p->key == NULL) {
      errorExit();
    }
    p->count = 0;
  }
  p->count++;
}

void printCounts(counts_t * c, FILE * outFile) {
  size_t i;
  one_count_t *p;

  for (i = 0; i < c->len; i++) {
    p = &c->base[i];
    fprintf(outFile, "%s: %d\n", p->key, p->count);
  }
  if (c->unknowns > 0) {
    fprintf(outFile, "<unknown> : %d\n", c->unknowns);
  }
}

void freeCounts(counts_t * c) {
  size_t i;
  one_count_t *p;

  for (i = 0; i < c->len; i++) {
    p = &c->base[i];
    free(p->key);
  }
  free(c->base);
  free(c);
}

// vim: ts=2:sw=2:et
