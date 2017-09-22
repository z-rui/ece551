all: printStates printEVotes printRecounts printLargeWin

print%: print%.o election.c
	gcc -ggdb3 -Wall -Werror -std=gnu99 election.c $< -o $@

clean:
	rm -f *~ printStates printEVotes printRecounts printLargeWin

