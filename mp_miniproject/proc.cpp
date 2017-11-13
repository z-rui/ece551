#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for waitpid */
#include <unistd.h> /* for fork */
#include <stdio.h> /* for perror */
#include <stdlib.h> /* for exit */

#include "myshell.h"

/* MyShell::runProgram runs the program specified by filename,
 * and passes to the program the arguments in argv
 * and environment variables in envp.
 *
 * It returns true if the program exited,
 * or false if the program was signaled.
 *
 * The integer pointed to by status is either set to the exit status
 * or the signal that caused the program to terminate.
 */
bool MyShell::runProgram(
	const char *filename,
	const char *const *argv,
	const char *const *envp,
	int *status)
{
	pid_t pid;

	pid = fork();
	if (pid == 0) { // in child process
		// I believe execve is wrong about the const-ness.
		// In fact both the characters and the pointers
		// will not be modified.
		execve(filename,
			const_cast<char *const *>(argv),
			const_cast<char *const *>(envp));
		// if execve does return, then there is an error
		// CITE: happens to be the same as in the man page
		perror("execve");
		exit(EXIT_FAILURE);
	} else { // in parent process
		int s, w;
		w = waitpid(pid, &s, 0);
		if (w == -1) { // CITE: from the man page
			perror("waitpid");
			exit(EXIT_FAILURE);
		}
		if (WIFEXITED(s)) {
			*status = WEXITSTATUS(s);
			return true;
		} else if (WIFSIGNALED(s)) {
			*status = WTERMSIG(s);
			return false;
		} else {
			throw Bug("unknown status from waitpid()");
		}
	}
}
