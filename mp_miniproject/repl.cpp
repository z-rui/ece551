#include <iostream>
#include <string>

#include <string.h>

#include "myshell.h"

/* prompt_input first writes the prompt string,
 * then reads a logical line for the shell.
 *
 * It returns true if std::cin is still good.
 */
bool MyShell::promptInput(std::string &line) const
{
	std::cout << "myShell:" << varTab.getVar("PWD") << " $ ";
	// Currently a logical line is just a physical line
	// (that is teminated by a '\n').
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
		bool ok = parser.parse(line.c_str(), pipes);
		if (!ok) {
			parser.reportSyntaxError(std::cout);
			continue;
		}
		if (pipes.size() == 0) { // empty
			continue;
		}
		if (pipes.size() > 1) {
			std::cout << "Pipes are not implemented yet.\n";
			continue;
		}
		Parser::Pipes::const_iterator it;
		it = pipes.begin();
		switch (it->type) {
			case Parser::Command::SET:
				executeSet(*it);
				break;
			case Parser::Command::EXPORT:
				executeExport(*it);
				break;
			case Parser::Command::CD:
				executeCd(*it);
				break;
			case Parser::Command::ORDINARY:
				executePipes(pipes);
				break;
			default:
				throw Bug("should have reported syntax error");
		}
	}
}

void MyShell::executePipes(const Parser::Pipes& pipes)
{
	Parser::Pipes::const_iterator it;
	it = pipes.begin();
	const char *progname = it->argv[0];

	progname = pathSearcher.search(progname);
	if (progname == NULL) {
		std::cout << "Command " << it->argv[0] <<
			" not found" << std::endl;
		return;
	}

	int status;
	bool exited;
	exited = runProgram(progname,
			&it->argv[0], // argv
			varTab.getExported(), // envp
			it->redir,
			&status);
	std::cout << "Program " <<
		(exited ? "exited with status "
			: "was killed by signal ") <<
		status << std::endl;
}

void MyShell::executeSet(const Parser::Command& cmd)
{
	const char *name = cmd.argv[1];
	const char *value = cmd.argv[2];

	if (strcmp(name, "PWD") == 0) {
		chdir(value);
		return; // PWD is set in chdir()
	}
	varTab.setVar(name, value);
	if (strcmp(name, "PATH") == 0) {
		pathSearcher.setPath(value);
	}
}

void MyShell::executeExport(const Parser::Command& cmd)
{
	const char *name = cmd.argv[1];
	varTab.exportVar(name);
}

void MyShell::executeCd(const Parser::Command& cmd)
{
	const char *path = cmd.argv[1];
	chdir(path);
}
