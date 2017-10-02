#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kv.h"

static
void errorExit(void)
{
  perror("kv");
  exit(EXIT_FAILURE);
}

static
void parseLine(kvpair_t *pair, const char *line)
{
  const char *p, *q;

  p = strchr(line, '=');
  if (p == NULL) {
    goto fail;
  }

  pair->key = strndup(line, (size_t) (p - line));
  q = strchr(++p, '\n');
  if (q == NULL) {
    goto fail;
  }
  pair->value = strndup(p, (size_t) (q - p));
  if (pair->key == NULL || pair->value == NULL) {
    goto fail;
  }
  return;
fail:
  fprintf(stderr, "invalid input format\n");
  exit(EXIT_FAILURE);
}

kvarray_t * readKVs(const char * fname) {
  FILE *f;
  char *line = NULL;
  size_t linesz = 0;
  kvpair_t *base = 0;
  size_t npairs = 0;
  kvarray_t *arr;

  f = fopen(fname, "r");
  if (f == NULL) {
    errorExit();
  }

  while (getline(&line, &linesz, f) != -1) {
    base = realloc(base, (npairs+1) * sizeof *base);
    if (base == NULL) {
      errorExit();
    }
    parseLine(&base[npairs++], line);
  }
  if (!feof(f)) {
    errorExit();
  }
  free(line);
  fclose(f);

  arr = malloc(sizeof *arr);
  if (arr == NULL) {
    errorExit();
  }
  arr->base = base;
  arr->len = npairs;
  return arr;
}

void freeKVs(kvarray_t * pairs) {
  size_t i;

  for (i = 0; i < pairs->len; i++) {
    free(pairs->base[i].key);
    free(pairs->base[i].value);
  }
  free(pairs->base);
  free(pairs);
}

void printKVs(kvarray_t * pairs) {
  size_t i;
  kvpair_t *p;

  for (i = 0; i < pairs->len; i++) {
    p = &pairs->base[i];
    printf("key = '%s' value = '%s'\n", p->key, p->value);
  }
}

char * lookupValue(kvarray_t * pairs, const char * key) {
  size_t i;
  kvpair_t *p;

  for (i = 0; i < pairs->len; i++) {
    p = &pairs->base[i];
    if (strcmp(key, p->key) == 0) {
      return p->value;
    }
  }
  return NULL;
}

// vim: ts=2:sw=2:et
