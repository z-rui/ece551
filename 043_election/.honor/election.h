#ifndef ELECTION_H
#define ELECTION_H
#include <stdint.h>
#include <stdlib.h>
#define MAX_STATE_NAME_LENGTH 64


struct _state_t {
  char name[MAX_STATE_NAME_LENGTH];
  uint64_t population;
  unsigned int electoralVotes;
};
typedef struct _state_t state_t;

state_t parseLine(const char * line);
unsigned int countElectoralVotes(state_t * stateData, 
				 uint64_t * voteCounts, 
				 size_t nStates);

void printRecounts(state_t * stateData, 
		   uint64_t * voteCounts, 
		   size_t nStates);
void printLargestWin(state_t * stateData, 
		     uint64_t * voteCounts, 
		     size_t nStates);

#endif
