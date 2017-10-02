#ifndef __KV_H__
#define __KV_H__


struct _kvpair_t { 
  char *key, *value;
};
typedef struct _kvpair_t kvpair_t;

struct _kvarray_t { 
  kvpair_t *base;
  size_t len;
};
typedef struct _kvarray_t kvarray_t;


kvarray_t * readKVs(const char * fname);

void freeKVs(kvarray_t * pairs);

void printKVs(kvarray_t * pairs);

char * lookupValue(kvarray_t * pairs, const char * key);

#endif
// vim: ts=2:sw=2:et
