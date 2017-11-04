#ifndef __READ_FREQ_H__
#define __READ_FREQ_H__
#include <stdint.h>
#include <assert.h>
#include <iostream>

uint64_t * readFrequencies(const char * fname);
void printSym(std::ostream & s, unsigned sym);
#endif
