#ifndef MYSHELL_H
#define MYSHELL_H

#include <vector>
#include <string>
#include <exception>

#include "path.h"
#include "parse.h"

class MyShell {
	PathSearcher pathSearcher;
	Parser parser;
	VarTab varTab;
	/* pipefd stores the fd of two pipes.
	 * pipefd[0]: the pipe that precedes the command
	 *            ({-1, -1} for the first command);
	 * pipefd[1]: the pipe that follows the command.
	 *            ({-1, -1} for the last command);
	 */
	int pipefd[2][2];

	static int (MyShell::*const executeCommand[])(const Parser::Command&);

	int executeSet(const Parser::Command&);
	int executeExport(const Parser::Command&);
	int executeCd(const Parser::Command&);
	int runExternal(const Parser::Command&);

	void executePipes(const Parser::Pipes&);
	bool promptInput(std::string &line) const;
	int chdir(const char *);
	void updatePWD();
public:
	MyShell();
	void runREPL();
};

#endif /* MYSHELL_H */
