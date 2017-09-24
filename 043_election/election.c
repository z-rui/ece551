#include "election.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <float.h>
//include any other headers you need here...

state_t parseLine(const char * line)
{
	state_t state;
	const char *save, *p;
	size_t namelen;

	save = p = line;

	/* name = [^:]+ */
	/* I don't see an accurate spec,
	 * but I would argue that this field
	 * should be non-empty */
	while (*p != ':') {
		if (*p == '\0') goto error;
		p++;
	}
	namelen = (size_t) (p - save);
	if (namelen == 0) goto error;
	if (namelen > MAX_STATE_NAME_LENGTH - 1) {
		fprintf(stderr, "name too long: %zu > %zu\n",
			namelen, (size_t) (MAX_STATE_NAME_LENGTH-1));
		exit(EXIT_FAILURE);
	}
	memcpy(state.name, save, namelen);
	state.name[namelen] = '\0';

	save = ++p; /* skip : */

	/* population = [0-9]+ */
	/* I'm also not sure if there're any constraints
	 * on the range of the input, e.g. must be positive? */
	state.population = 0ULL;
	while ('0' <= *p && *p <= '9') {
		state.population = state.population * 10ULL + (uint64_t) (*p - '0');
		p++;
	}
	if (p == save || *p != ':') goto error;

	save = ++p; /* skip : */

	/* electoralVotes = [0-9]+ */
	state.electoralVotes = 0U;
	while ('0' <= *p && *p <= '9') {
		state.electoralVotes = state.electoralVotes * 10U + (unsigned) (*p - '0');
		p++;
	}
	if (p == save || *p != '\0') goto error;

	/* success */
	return state;

error:
	fprintf(stderr, "unexpected character \\x%02x at position %td.\n",
		*p, p - line);
	fprintf(stderr, "%s\n%*s\n", line, (int) (p - line + 1), "^");
	exit(EXIT_FAILURE);
}

unsigned int countElectoralVotes(state_t * stateData, uint64_t * voteCounts, size_t nStates)
{
	unsigned count = 0;
	size_t i;

	for (i = 0; i < nStates; i++) {
		/* no 2*x > y (potential overflow),
		 * no x > 0.5*y (insufficient accuracy) */
		if (voteCounts[i] > stateData[i].population / 2) {
			count += stateData[i].electoralVotes;
		}
	}

	return count;
}

void printRecounts(state_t * stateData, uint64_t * voteCounts, size_t nStates)
{
	size_t i;

	for (i = 0; i < nStates; i++) {
		uint64_t pop, vote;
		uint64_t halfpop, pop_by_200;
		uint64_t lower, upper;

		pop = stateData[i].population;
		vote = voteCounts[i];

		/* it's easy to say, e.g., vote >= 0.495 * pop && vote <= 0.505 * pop,
		 * but uint64_t -> double is not always exact. */
		halfpop = pop / 2;
		pop_by_200 = pop / 200; /* 1/200 = 0.5% */

		lower = halfpop - pop_by_200;
		upper = halfpop + pop_by_200;
		if (pop % 2 == 1) { /* fix rounding issues */
			if (pop % 200 < 100) {
				lower++;
			} else {
				upper++;
			}
		}

		if (vote >= lower && vote <= upper) {
			printf("%s requires a recount (Candidate A has %.2f%% of the vote)\n",
				stateData[i].name, 100.0 * vote / pop);
		}
	}
}

void printLargestWin(state_t * stateData, uint64_t * voteCounts, size_t nStates)
{
	size_t i;
	size_t largestIdx;
	long double this_percentage, largest_percentage = 0.0L;

	/* Doing this while avoiding floating point is too hard.  :( */

	for (i = 0; i < nStates; i++) {
		this_percentage = (long double) voteCounts[i] / stateData[i].population;
		if (this_percentage > largest_percentage) {
			largestIdx = i;
			largest_percentage = this_percentage;
		}
	}
	printf("Candidate A won %s with %.2f%% of the vote\n",
		stateData[largestIdx].name, (double) largest_percentage * 100.0);
}
