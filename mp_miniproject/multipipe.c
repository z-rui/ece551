#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>

char *arg[] = {
	"/bin/dd", "bs=4096", "count=4096", "if=/dev/zero", "of=/dev/stdout", 0,
	//"/bin/cat", 0,
	"/usr/bin/wc", "-c", 0,
};

char **argv[] = {
	arg,
	arg + 6,
	//arg + 8,
	0,
};

int main()
{
	int prev[2] = {-1, -1}, next[2];
	char ***argvp;
	pid_t pid;

	for (argvp = argv; *argvp; argvp++) {
		if (argvp[1]) {
			if (pipe(next) == -1) {
				perror("pipe");
				return 1;
			}
		} else {
			next[0] = next[1] = -1;
		}
		if ((pid = fork()) == -1) {
			perror("fork");
			return 1;
		}
		if (pid == 0) {
			if (prev[0] != -1) {
				close(prev[1]);
				dup2(prev[0], 0);
			}
			if (next[0] != -1) {
				close(next[0]);
				dup2(next[1], 1);
			}
			if (execv(**argvp, *argvp) == -1) {
				perror("execv");
			}
			return 1;
		}
		if (prev[0] != -1) {
			close(prev[0]);
			close(prev[1]);
		}
		prev[0] = next[0];
		prev[1] = next[1];
	}
	while (wait(0) != -1)
		;
	if (errno != ECHILD)
		perror("wait");
	return 0;
}
