#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for waitpid */
#include <unistd.h> /* for fork */
#include <fcntl.h>

#include <vector>

#include <stdio.h> /* for perror */
#include <stdlib.h> /* for exit */
#include <assert.h>

#include "myshell.h"

static void dup2_or_die(int oldfd, int newfd)
{
	if (dup2(oldfd, newfd) == -1) {
		perror("dup2");
		exit(EXIT_FAILURE);
	}
}

static int open_or_die(const char *path, int flags)
{
	int fd = open(path, flags);
	if (fd == -1) {
		perror("open");
		exit(EXIT_FAILURE);
	}
	return fd;
}

static void setupChild(
	const char *path,
	const char *const *argv,
	const char *const *envp,
	const char *const redir[3],
	const int prevPipe[2],
	const int nextPipe[2])
{
	if (prevPipe[0] != -1) {
		close(prevPipe[1]);
		dup2_or_die(prevPipe[0], 0);
	}
	if (nextPipe[0] != -1) {
		close(nextPipe[0]);
		dup2_or_die(nextPipe[1], 1);
	}
	if (redir[0]) {
		int fd = open_or_die(redir[0], O_RDONLY);
		dup2_or_die(fd, 0);
	}
	for (int i = 1; i <= 2; i++) {
		if (redir[i]) {
			int fd = creat(redir[i], 0666);
			if (fd == -1) {
				perror("creat");
				exit(EXIT_FAILURE);
			}
			dup2_or_die(fd, i);
		}
	}
	// I believe execve is wrong about the const-ness.
	// In fact both the characters and the pointers
	// will not be modified.
	execve(path,
		const_cast<char *const *>(argv),
		const_cast<char *const *>(envp));
	// if execve does return, then there is an error
	perror("execve");
	exit(EXIT_FAILURE);
}

int MyShell::runExternal(const Parser::Command& cmd)
{
	const char *progname = cmd.argv[0];
	if (progname == NULL) {
		// weird command like "<FILE" ?
		return -1;
	}
	const char *path = pathSearcher.search(progname);
	if (path == NULL) {
		std::cout << "Command " << progname
			<< " not found\n";
		return -1;
	}
	pid_t pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}
	if (pid == 0) {
		setupChild(path, &cmd.argv[0], varTab.getExported(),
			cmd.redir, pipefd[0], pipefd[1]);
		// will not reach here
	}
	return pid;
}

void MyShell::executePipes(const Parser::Pipes& pipes)
{
	if (pipes.empty()) {
		return;
	}

	Parser::Pipes::const_iterator it, nextit;
	std::vector<int> children;

	pipefd[0][0] = pipefd[0][1] = -1;
	for (it = nextit = pipes.begin(); it != pipes.end(); it = nextit) {
		++nextit;
		bool run = true;
		if (nextit == pipes.end()) {
			pipefd[1][0] = pipefd[1][1] = -1;
		} else if (pipe(pipefd[1]) == -1) {
			perror("pipe");
			run = false;
		}
		if (run) {
			assert(it->type < Parser::Command::INVALID);
			pid_t pid = (this->*executeCommand[it->type])(*it);
			if (pid != -1) {
				children.push_back(pid);
			}
		}
		if (pipefd[0][0] != -1) {
			close(pipefd[0][0]);
			close(pipefd[0][1]);
		}
		pipefd[0][0] = pipefd[1][0];
		pipefd[0][1] = pipefd[1][1];
	}
	for (size_t i = 0; i < children.size(); i++) {
		int status;
		if (waitpid(children[i], &status, 0) == -1) {
			perror("waitpid");
			continue;
		}
		if (WIFEXITED(status)) {
			std::cout << "Program exited with status "
				<< WEXITSTATUS(status);
		} else if (WIFSIGNALED(status)) {
			std::cout << "Program was killed by signal "
				<< WTERMSIG(status);
		}
		if (children.size() > 1) {
			std::cout << " (PID = " << children[i] << ")";
		}
		std::cout << std::endl;
	}
}
