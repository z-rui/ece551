#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "outname.h"

char * computeOutputFileName(const char * inputName) {
  size_t len;
  char *outName;

  len = strlen(inputName);
  outName = malloc(len + 8); // . c o u n t s \0
  memcpy(outName, inputName, len);
  memcpy(outName + len, ".counts", 8);
  return outName;
}

// vim: ts=2:sw=2:et
