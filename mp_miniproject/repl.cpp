#include <iostream>
#include <string>

#include <string.h>
#include <assert.h>

#include "myshell.h"
#include "hash.h"

/* prompt_input first writes the prompt string,
 * then reads a logical line for the shell.
 *
 * It returns true if std::cin is still good.
 */
bool MyShell::promptInput(std::string &line) const
{
	std::cout << "myShell:" << varTab.getVar("PWD") << " $ ";
	// Currently a logical line is just a physical line
	// (that is what std::getline gets).
	// But I can make extensions in the future.
	std::getline(std::cin, line);
	return std::cin.good();
}

/* MyShell::runREPL runs the Read-Evaluate-Print Loop.
 */
void MyShell::runREPL()
{
	std::string line;
	while (promptInput(line)) {
		if (line == "exit") {
			break;
		}
		Parser::Pipes pipes;
		if (parser.parse(line.c_str(), pipes)) {
			executePipes(pipes);
		} else {
			parser.reportSyntaxError(std::cout);
		}
	}
}


/* MyShell::executeCommand is the "jump table" for the shell
 * so that it can call the appropriate method to execute
 * a command, according to its type.
 */

int (MyShell::*const MyShell::executeCommand[])(const Parser::Command&) = {
	[Parser::Command::SET] = &MyShell::executeSet,
	[Parser::Command::EXPORT] = &MyShell::executeExport,
	[Parser::Command::CD] = &MyShell::executeCd,
	[Parser::Command::ORDINARY] = &MyShell::runExternal,
};

/* A few methods to execute builtin commands follow.
 * They all return -1 because no child process is created.
 */

/* MyShell::executeSet executes the set command.
 *
 * It also takes care of some special variables.
 */
int MyShell::executeSet(const Parser::Command& cmd)
{
	assert(cmd.argv.size() == 4);
	const char *name = cmd.argv[1];
	const char *value = cmd.argv[2];

	if (strcmp(name, "PWD") == 0) {
		chdir(value); // chdir() will update PWD, or fail
		return -1;
	}
	varTab.setVar(name, value);
	if (strcmp(name, "PATH") == 0) {
		pathSearcher.setPath(value);
	}
	return -1;
}

/* MyShell::executeExport executes the export command.
 */
int MyShell::executeExport(const Parser::Command& cmd)
{
	assert(cmd.argv.size() == 3);
	const char *name = cmd.argv[1];
	varTab.exportVar(name);
	return -1;
}

/* MyShell::executeCd executes the cd command.
 */
int MyShell::executeCd(const Parser::Command& cmd)
{
	assert(cmd.argv.size() == 3);
	const char *path = cmd.argv[1];
	chdir(path);
	return -1;
}
