#ifndef __COUNTS_H__
#define __COUNTS_H__
struct _one_count_t {
  char *key;
  int count;
};
typedef struct _one_count_t one_count_t;

struct _counts_t {
  one_count_t *base;
  size_t len;
  int unknowns;
};
typedef struct _counts_t counts_t;

counts_t * createCounts(void);
void addCount(counts_t * c, const char * name);
void printCounts(counts_t * c, FILE * outFile);

void freeCounts(counts_t * c);

#endif

// vim: ts=2:sw=2:et
